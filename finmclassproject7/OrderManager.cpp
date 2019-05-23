//
// Created by sebastiend on 06/10/2018.
//

#include "OrderManager.h"


bool OrderManager::handle_execution_order(){
    if(!is_working)
        return false;
    if (simulator_to_ordermanager.empty())
        return true;

    const ExecutionOrder &e = simulator_to_ordermanager.front();
    ordermanager_to_strategy.push(e);
    simulator_to_ordermanager.pop();

    return true;
};


unsigned int OrderManager::get_number_of_open_orders()
{
    return 0;
}


unsigned int OrderManager::get_number_of_non_acknowledged_orders()
{
    return 0;
}

int OrderManager::get_position(std::string symbol)
{
    return 0;
}

bool OrderManager::handle_order(){
    if(!is_working)
        return false;
    if (strategy_to_ordermanager.empty())
        return true;
    const Order &e = strategy_to_ordermanager.front();

    order_id++;
    order=e;
    //std::cout << "TRY ORDER..." << list_orders.size() << std::endl;

    order.setOrderID(order_id);
    //std::cout << "OMS push an order" << std::endl;
    ordermanager_to_simulator.push(order);
    strategy_to_ordermanager.pop();

    return true;
};
