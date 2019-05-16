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
    simulator_to_ordermanager.pop();
    ordermanager_to_strategy.push(e);
    orderstate state = e.getState();
    switch (state) {
        case FILLED:
            positions[e.getSymbol()] += e.getPrice() * e.getQuantity() * (e.isBuy() ? 1 : -1);
        case REJECTED:
        case CANCELLED:
            list_orders.erase(e.getID());
            break;
        case OPEN:
        case ACKNOWLEDGED:
            list_orders[e.getID()] = ExecutionOrder(e);
    }
    return true;
};


unsigned int OrderManager::get_number_of_open_orders()
{
    int num = 0;
    for (auto &it : list_orders)
    {
        orderstate state = it.second.getState();
        if (state == orderstate::OPEN || state == orderstate::ACKNOWLEDGED)
            num++;
    }
    return num;
}


unsigned int OrderManager::get_number_of_non_acknowledged_orders()
{
    int num = 0;
    for (auto &it : list_orders)
    {
        if (it.second.getState() == orderstate::OPEN)
            num++;
    }
    return num;
}

int OrderManager::get_position(std::string symbol)
{
    return positions[symbol];
}

bool OrderManager::handle_order(){
    if(!is_working)
        return false;
    if (strategy_to_ordermanager.empty())
        return true;
    const Order &e = strategy_to_ordermanager.front();
    strategy_to_ordermanager.pop();

    int qty = e.getQuantity();
    int pos = qty * e.getPrice() * (e.isBuy() ? 1 : -1);
    if (qty < 10 or list_orders.size() > 10 or std::abs(pos + positions[e.getSymbol()]) > 1000000) {
        ExecutionOrder f(e);
        f.setState(orderstate::REJECTED);
        ordermanager_to_strategy.push(f);
        return false;
    }
    ordermanager_to_simulator.push(e);
    list_orders.insert(std::make_pair(e.getID(), e));
    return true;
};
