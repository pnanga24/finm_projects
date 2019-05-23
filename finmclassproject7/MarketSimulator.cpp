//
// Created by sebastiend on 06/10/2018.
//

#include "MarketSimulator.h"


MarketSimulator::MarketSimulator(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_,
        std::queue<BookUpdate> &bookbuilder_to_strategy_
        ):AppBase(strategy_to_ordermanager_,
                ordermanager_to_strategy_,
                ordermanager_to_simulator_,
                 simulator_to_ordermanager_,
                 bookbuilder_to_strategy_),
                 execution_id(0),
                 pnl(0)
     {
        list_symbols = {"EUR/USD"};
        log.open("log.csv", std::ios::out | std::ios::trunc);
        if (!log.is_open())
            throw std::runtime_error("Cannot open log file");
        log << "Time,Buy,Price,Qty,PNL" << std::endl;
     }


bool MarketSimulator::handle_order(){
    if(!is_working)
        return false;
    if(ordermanager_to_simulator.empty())
        return true;
    const Order &o = ordermanager_to_simulator.front();
    ExecutionOrder new_execution(o);
    const bool is_tradeable = list_symbols.find(o.getSymbol()) != list_symbols.end();
    ordermanager_to_simulator.pop();

    if(is_tradeable){
        new_execution.setState(
                new_execution.getQuantity()>1000
                ? orderstate::ACKNOWLEDGED
                : orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
        if (new_execution.getState() == orderstate::REJECTED)
            return true;
        new_execution.setState(orderstate::FILLED);
        pnl += new_execution.getQuantity() * new_execution.getPrice()
                * (new_execution.isBuy() ? -1 : 1);
        std::string msg = std::to_string(new_execution.getTimeStamp()) + "," +
                std::to_string(new_execution.isBuy()) + "," +
                std::to_string(new_execution.getPrice()) + "," +
                std::to_string(new_execution.getQuantity()) + "," +
                std::to_string(pnl);
        std::cout << "simulator push a fill|" << msg << std::endl;
        log << msg << std::endl;
        simulator_to_ordermanager.push(new_execution);
    }
    else{
        // std::cout << "simulator push a reject" << std::endl;
        new_execution.setState(orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
    }
    return true;
}


double MarketSimulator::get_pnl() {
    return pnl;
};