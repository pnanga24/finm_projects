//
// Created by sebastiend on 21/09/2018.
//

#ifndef OEOR_HOMEWORK2_MY_CLASSES_H
#define OEOR_HOMEWORK2_MY_CLASSES_H

#include <string.h>
#include <iostream>
#include <stdlib.h>

enum ordertype {LIMIT, MARKET};


class Order{
private:
    long timestamp; // epoch time: the number of seconds that have elapsed since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970,.
    bool is_buy; // buy or sell
    unsigned int id; // order id – unique identifier
    unsigned int price;
    unsigned int quantity;
    char venue[20]; // company name where this order comes from
    char symbol[20];
    ordertype type;
public:

    Order(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
          unsigned int quantity_, const char * venue_, const char * symbol_,
          ordertype type_) : timestamp(timestamp_), is_buy(is_buy_), id(id_), price(price_),
                             quantity(quantity_), type(type_)
    {
        strcpy(venue, venue_);
        strcpy(symbol, symbol_);
    }

    Order (const Order& o) : timestamp(o.timestamp), is_buy(o.is_buy), id(o.id), price(o.price),
    quantity(o.quantity), type(o.type)
    {
        strcpy(venue, o.venue);
        strcpy(symbol, o.symbol);
    }

    char * getVenue()
    {
        return venue;
    }

    char * getSymbol()
    {
        return symbol;
    }

    unsigned int getID()
    {
        return id;
    }

    ordertype getOrderType()
    {
        return type;
    }

    unsigned int getQuantity()
    {
        return quantity;
    }

    unsigned int getPrice()
    {
        return price;
    }

    bool is_valid()
    {
        return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
    }

    void setVenue(const char * venue_)
    {
        strcpy(venue,venue_);
    }

    void setQuantity(unsigned int quantity_)
    {
        quantity=quantity_;
    }

    virtual int getOutstandingQuantity(){return getQuantity();}

};

class ClosedOrder: public Order {
public:
    ClosedOrder(long timestamp_,
          bool is_buy_,
          unsigned int id_,
          unsigned int price_,
          unsigned int quantity_,
          const char * venue_,
          const char * symbol_,
          ordertype type_) :
    Order(timestamp_, is_buy_, id_, price_,
        quantity_, venue_, symbol_, type_) {}

    int getOutstandingQuantity() {return 0;}
};

class OpenOrder: public Order {
public:
    OpenOrder(long timestamp_,
                bool is_buy_,
                unsigned int id_,
                unsigned int price_,
                unsigned int quantity_,
                const char * venue_,
                const char * symbol_,
                ordertype type_) :
            Order(timestamp_, is_buy_, id_, price_,
                  quantity_, venue_, symbol_, type_) {}

    int getOutstandingQuantity() {return getQuantity();}
};



class VectorOrders{
private:
    Order **orders;
    unsigned int capacity;
    unsigned int current_new_order_offset;
public:
    VectorOrders(int capacity_) :
        capacity(capacity_),
        current_new_order_offset(0) {
        orders = new Order*[capacity]{nullptr};
    }

    VectorOrders(const VectorOrders& vec) {
        capacity = vec.capacity;
        current_new_order_offset = vec.current_new_order_offset;
        orders = new Order*[capacity]{nullptr};
        for (int i=0; i<vec.current_new_order_offset; i++) {
            orders[i] = new Order(*vec.orders[i]);
        }
    }

    ~VectorOrders() {
        clear();
        delete[] orders;
    }

    bool add_order(Order * o);
    bool delete_order(unsigned int id);
    int get_total_volume();
    int get_total_outstanding_volume();


    Order **get_order_list() const
    {
        return orders;
    }

    bool double_list_orders_size()
    {
        // You will use this function
        // To reallocate an the array and doubling its size
        Order **new_orders = new Order *[capacity*2]{nullptr};
        std::copy(orders, orders + current_new_order_offset, new_orders);
        capacity *= 2;
        delete[] orders;
        orders = new_orders;
        return true;
    }

    unsigned int get_size()
    {
        return current_new_order_offset;
    }

    unsigned int get_capacity()
    {
        return capacity;
    }

    void clear()
    {
        for (int i=0; i<current_new_order_offset; i++) {
            delete orders[i];
            orders[i] = nullptr;
        }
        current_new_order_offset = 0;
    }

    void dump_list_orders();
};


#endif //OEOR_HOMEWORK2_MY_CLASSES_H
