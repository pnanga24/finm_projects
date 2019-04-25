//
// Created by sebastiend on 21/09/2018.
//

#include "my_classes.h"

OrderBook::~OrderBook() {
    clearBooks();
}

bool OrderBook::add_order(long timestamp_,
               bool is_buy_,
               unsigned int id_,
               unsigned int price_,
               unsigned int quantity_,
               const char * venue_,
               const char * symbol_,
               ordertype type_)
{
    auto &orders = is_buy_ ? order_bids : order_offers;
    if (orders[id_] == nullptr) {
        orders[id_] = new Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_);
        int &num = is_buy_ ? number_of_bids : number_of_offers;
        num++;
        return true;
    } else {
        return false;
    }
}
bool OrderBook::modify_order(bool is_buy_,
                  unsigned int id_,
                  unsigned int quantity_)
{
    auto &orders = is_buy_ ? order_bids : order_offers;
    if (orders[id_] != nullptr) {
        orders[id_]->setQuantity(quantity_);
        return true;
    } else {
        return false;
    }

}
bool OrderBook::remove_order(bool is_buy_, unsigned int id_)
{
    auto &orders = is_buy_ ? order_bids : order_offers;
    if (orders[id_] != nullptr) {
        delete orders[id_];
        int &num = is_buy_ ? number_of_bids : number_of_offers;
        num--;
        orders[id_] = nullptr;
        return true;
    } else {
        return false;
    }
}
void OrderBook::clearBooks()
{
    for(int k=0;k<20;k++)
    {
        if (order_bids[k]!=nullptr)
        {
            delete order_bids[k];
            order_bids[k]=nullptr;
            number_of_bids--;
        }
        if (order_offers[k]!=nullptr)
        {
            delete order_offers[k];
            order_offers[k]=nullptr;
            number_of_offers--;
        }
    }
}