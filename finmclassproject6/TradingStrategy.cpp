//
// Created by sebastiend on 06/10/2018.
//

#include "TradingStrategy.h"


bool Execution::insert_order(long timestamp_,
                  bool is_buy_,
                  unsigned int price_,
                  unsigned int quantity_,
                  const char * venue_,
                  const char * symbol_,
                  ordertype type_,
                  unsigned int id_)
{
    e.setSide(is_buy_);
    e.setPrice(price_);
    e.setQuantity(quantity_);
    e.setVenue(venue_);
    e.setType(type_);
    e.setSymbol(symbol_);
    e.setOrderID(id_);
};

bool TradingStrategy::process_book_update(BookUpdate &bu){
    if(!is_working)
        return false;
    if (signal.is_tradeable(bu)) {
        execution.insert_order(0,bu.get_is_buy(),
                               bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
        execution.set_tradeable(true);
    }
    return process_execution();
}

bool TradingStrategy::process_execution() {
    if(!is_working)
        return false;
    Order order;
    if (execution.is_tradeable()) {
        order.setType(execution.get_order().getOrderType());
        order.setVenue(execution.get_order().getVenue());
        order.setQuantity(execution.get_order().getQuantity());
        order.setPrice(execution.get_order().getPrice());
        order.setOrderID(execution.get_order().getID());
        order.setSymbol(execution.get_order().getSymbol());
        order.setSide(execution.get_order().isBuy());
        execution.set_tradeable(false);
        strategy_to_ordermanager.push(order);
    }
    return true;
};

bool TradingStrategy::process_market_response()
{
    if(ordermanager_to_strategy.empty())
        return true;
    ExecutionOrder &order = ordermanager_to_strategy.front();
    ordermanager_to_strategy.pop();

    return true;
}

int TradingStrategy::get_position(std::string symbol)
{
        return 0;
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return 0;
}

unsigned int TradingStrategy::get_number_of_fills() {
    return 0;
}

void TradingStrategy::reset_position(){
    positions.clear();
}