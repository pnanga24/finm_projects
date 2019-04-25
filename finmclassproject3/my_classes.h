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
    long timestamp;
    bool is_buy;
    unsigned int id;
    unsigned int price;
    unsigned int quantity;
    char venue[20];
    char symbol[20];
    ordertype type;
public:
    Order() {};
    Order(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
          unsigned int quantity_, const char * venue_, const char * symbol_,
          ordertype type_) : timestamp(timestamp_), is_buy(is_buy_), id(id_), price(price_),
          quantity(quantity_), type(type_)
    {
        strcpy(venue, venue_);
        strcpy(symbol, symbol_);
    }

    ~Order() {};

    char * getVenue() {return venue;} // will return the venue string
    char * getSymbol() {return symbol;} // will return the symbol string
    unsigned int getID() {return id;} // get the order id
    ordertype getOrderType() {return type;} // get the order type
    unsigned int getQuantity() {return quantity;} // get the quantity
    unsigned int getPrice() {return price;} // get the price
    bool is_valid() // check if order is valid
    {
        return (price >= 0) && (quantity >=0) && (venue[0] != '\0');
    }
    void setVenue(const char * venue_) {*venue = *venue_;} // change the venue
    void setQuantity(unsigned int quantity_) {quantity = quantity_;} // change the quantity
};

class OrderBook{
private:
    Order * order_bids[20];
    Order * order_offers[20];
    int number_of_bids;
    int number_of_offers;

public:
    OrderBook() : number_of_bids(0), number_of_offers(0), order_bids{nullptr}, order_offers{nullptr} {}
    ~OrderBook();

    bool add_order(long timestamp_,
                   bool is_buy_,
                   unsigned int id_,
                   unsigned int price_,
                   unsigned int quantity_,
                   const char * venue_,
                   const char * symbol_,
                   ordertype type_);

    bool modify_order(bool is_buy_,
                      unsigned int id_,
                      unsigned int quantity_
    );

    bool remove_order(bool is_buy_, unsigned int id_);

    void clearBooks();
    Order* getOrderBids(const int a)
    {
        return order_bids[a];
    }
    Order* getOrderOffers(const int a)
    {
        return order_offers[a];
    }
    int getNumberOfBids()
    {
        return number_of_bids;
    }
    int getNumberOfOffers()
    {
        return number_of_offers;
    }
};

#endif //OEOR_HOMEWORK2_MY_CLASSES_H
