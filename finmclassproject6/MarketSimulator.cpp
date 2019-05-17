//
// Created by sebastiend on 06/10/2018.
//

#include "MarketSimulator.h"


MarketSimulator::MarketSimulator(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_
        ):AppBase(strategy_to_ordermanager_,
                ordermanager_to_strategy_,
                ordermanager_to_simulator_,
                 simulator_to_ordermanager_),
                 execution_id(0)
     {
        list_symbols = {"MSFT", "AAPL", "LUV", "SKYW","UAL","HA","RYAAY","DAL"};
     }


bool MarketSimulator::handle_order(){
    if(!is_working)
        return false;
    if(ordermanager_to_simulator.empty())
        return true;
    const Order &o = ordermanager_to_simulator.front();

    const bool is_tradeable = list_symbols.find(o.getSymbol()) != list_symbols.end();

    ExecutionOrder new_execution(o);
    if(is_tradeable){
        new_execution.setState(o.getQuantity()>1000?orderstate::ACKNOWLEDGED:orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
        ordermanager_to_simulator.pop();
        while (!ordermanager_to_simulator.empty() &&
                ordermanager_to_simulator.front().getID() == new_execution.getID()) {
            Order &p = ordermanager_to_simulator.front();
            if (order_match(p, new_execution))
                new_execution.setQuantity(p.getQuantity());
            ordermanager_to_simulator.pop();
        }
        new_execution.setState(orderstate::FILLED);
//            std::cout << "simulator push a fill|" <<
//                        new_execution.getPrice() << "|"  <<
//                        new_execution.isBuy()  << "|" <<
//                        new_execution.getQuantity() << "|" <<
//                        std::endl;
        simulator_to_ordermanager.push(new_execution);
    }
    else{
        // std::cout << "simulator push a reject" << std::endl;
        new_execution.setState(orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
        ordermanager_to_simulator.pop();
    }
    return true;
}

bool MarketSimulator::order_match(const Order &o, const Order &e)
{
    return (o.getPrice() == e.getPrice() && strcmp(o.getSymbol(), e.getSymbol()) == 0
            && strcmp(o.getVenue(), e.getVenue()) == 0);
}