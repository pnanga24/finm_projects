//
// Created by sebastiend on 06/10/2018.
//

#include <math.h>
#include "TradingStrategy.h"

void Signal::insert_book_update(BookUpdate bu) {
    if (bu.get_is_buy()) {
        double diff = get_5min_moving_average() - get_20min_moving_average();
        sign_diff_520 = (diff > 0) - (diff < 0);

        price_queue_5.push(std::make_pair(bu.get_timestamp(), bu.get_price()));
        price_queue_20.push(std::make_pair(bu.get_timestamp(), bu.get_price()));
    } else {
        double &mid5 = price_queue_5.back().second;
        double &mid20 = price_queue_20.back().second;
        mid5 = (mid5 + bu.get_price()) / 2;
        sum5 += mid5;
        mid20 = (mid20 + bu.get_price()) / 2;
        sum20 += mid20;
    }
    while (!price_queue_5.empty() && price_queue_5.front().first + 300 <= bu.get_timestamp()) {
        sum5 -= price_queue_5.front().second;
        price_queue_5.pop();
    }
    while (!price_queue_20.empty() && price_queue_20.front().first + 1200 <= bu.get_timestamp()) {
        sum20 -= price_queue_20.front().second;
        price_queue_20.pop();
    }
}

double Signal::get_5min_moving_average() {
    return price_queue_5.empty() ? 0 : round(100 * sum5 / price_queue_5.size()) / 100;
}

double Signal::get_20min_moving_average() {
    return price_queue_20.empty() ? 0 : round(100 * sum20 / price_queue_20.size()) / 100;
}

bool Signal::go_long() {
    return (sign_diff_520 != 1) && (get_5min_moving_average() > get_20min_moving_average());
}

bool Signal::go_short() {
    return (sign_diff_520 != -1) && (get_5min_moving_average() < get_20min_moving_average());
}


bool Execution::insert_order(long timestamp_,
                  bool is_buy_,
                  price_t price_,
                  unsigned int quantity_,
                  const char * venue_,
                  const char * symbol_,
                  ordertype type_,
                  unsigned int id_)
{
    e.setTimeStamp(timestamp_);
    e.setSide(is_buy_);
    e.setPrice(price_);
    e.setQuantity(quantity_);
    e.setVenue(venue_);
    e.setType(type_);
    e.setSymbol(symbol_);
    e.setOrderID(id_);
};

bool TradingStrategy::process_book_update(BookUpdate &bu) {
    if (!is_working)
        return false;
    signal.insert_book_update(bu);
    if (!bu.get_is_buy()) { // only evaluate signal after both bid and ask are inputted (cross market)
        market_prices[bu.get_symbol()].second = bu.get_price();
        if (signal.go_long()) {
            execution.insert_order(bu.get_timestamp(), true, bu.get_price(),
                    10000, bu.get_venue(), bu.get_symbol(), ordertype::LIMIT, order_id++);
            execution.set_tradeable(true);
        } else if (signal.go_short()) {
            execution.insert_order(bu.get_timestamp(), false, market_prices[bu.get_symbol()].first,
                    10000, bu.get_venue(), bu.get_symbol(), ordertype::LIMIT, order_id++);
            execution.set_tradeable(true);
        }
    } else
        market_prices[bu.get_symbol()].first = bu.get_price();
    return process_execution();
}


bool TradingStrategy::process_book_update(){
    if(!is_working)
        return false;
    if(bookbuilder_to_strategy.empty())
        return true;
    BookUpdate &bu = bookbuilder_to_strategy.front();
    bool ret = process_book_update(bu);
    bookbuilder_to_strategy.pop();
    return ret;
}

bool TradingStrategy::process_execution() {
    if(!is_working)
        return false;
    if (execution.is_tradeable()) {
        execution.set_tradeable(false);
        //std::cout << "strategy push an order" << std::endl;
        strategy_to_ordermanager.push(execution.get_order());
    }
    return true;
};

bool TradingStrategy::process_market_response()
{
    if(ordermanager_to_strategy.empty())
        return true;
    ExecutionOrder &order = ordermanager_to_strategy.front();
    switch (order.getState()) {
        case orderstate::FILLED: {
            int qty = order.getQuantity() * (order.isBuy() ? 1 : -1);
            positions[order.getSymbol()].first += qty;
            positions[order.getSymbol()].second -= order.getPrice() * qty;
            number_of_fills++;
            break;
        }
        case orderstate::REJECTED:
            number_of_rejections++;
            break;
    }
    ordermanager_to_strategy.pop();
    return true;
}

double TradingStrategy::get_position(std::string symbol)
{
    return positions[symbol].second;
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return number_of_rejections;
}

unsigned int TradingStrategy::get_number_of_fills() {
    return number_of_fills;
}

double TradingStrategy::get_pnl() {
    double pnl = 0;
    for (auto &it : positions) {
        pnl += it.second.second;
    }
    return pnl;
}

double TradingStrategy::get_m2m_pnl() {
    double pnl = 0;
    for (auto &it : positions) {
        int net_qty = it.second.first;
        double net_pos = it.second.second;
        std::pair<double, double> price = market_prices[it.first];
        // liquidate position by crossing market. net_qty > 0 => need to sell => price is bid
        double mkt_price = net_qty > 0 ? price.first : price.second;
        // double mkt_price = (price.first + price.second) / 2;
        pnl += net_pos + net_qty * mkt_price;
    }
    return pnl;
}

void TradingStrategy::reset_position(){
    positions.clear();
}