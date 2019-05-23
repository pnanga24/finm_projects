//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_TRADINGSTRATEGY_H
#define IEOR_HOMEWORK5_TRADINGSTRATEGY_H
#include "AppBase.h"
#include "BookUpdate.h"
#include "Order.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <queue>

class Signal{
private:
    std::queue<std::pair<unsigned int, double>> price_queue_5;
    std::queue<std::pair<unsigned int, double>> price_queue_20;
    double sum5;
    double sum20;
    int sign_diff_520;
public:
    Signal() : sum5(0), sum20(0), sign_diff_520(0) {};
    void insert_book_update(BookUpdate bu);
    double get_5min_moving_average();
    double get_20min_moving_average();
    bool go_long();
    bool go_short();
    bool is_tradeable(BookUpdate &bu){return true;}
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      price_t price_,
                      unsigned int quantity_,
                      const char * venue_,
                      const char * symbol_,
                      ordertype type_,
                      unsigned int id_);
    bool is_tradeable() {return tradeable;}
    void set_tradeable(bool is_tradable){tradeable=is_tradable;};
    Order & get_order() {return e;}
};

class TradingStrategy : public AppBase {
private:
    Signal signal;
    Execution execution;
    int order_id;
    std::unordered_map<std::string,std::pair<int, double>> positions;
    std::unordered_map<std::string, std::pair<double, double>> market_prices;
    unsigned int number_of_rejections;
    unsigned int number_of_fills;

public:
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_,
              bookbuilder_to_strategy_),
              signal(),
              execution(),
              order_id(1),
              number_of_rejections(0),
              number_of_fills(0) {}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
    }


    bool process_book_update(BookUpdate &bu);
    bool process_book_update();
    bool process_execution();
    bool process_market_response();
    double get_position(std::string symbol);
    unsigned int get_number_of_rejections();
    unsigned int get_number_of_fills();
    void reset_position();
    double get_pnl();
    double get_m2m_pnl();

};


#endif //IEOR_HOMEWORK5_TRADINGSTRATEGY_H
