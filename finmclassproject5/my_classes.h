//
// Created by sebastiend on 21/09/2018.
//

#ifndef OEOR_HOMEWORK2_MY_CLASSES_H
#define OEOR_HOMEWORK2_MY_CLASSES_H

#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <map>


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
    Order(long timestamp_=0,
            bool is_buy_=true,
            unsigned int id_=0,
            unsigned int price_=0,
            unsigned int quantity_=0,
            const char * venue_="",
            const char * symbol_="",
            ordertype type_=ordertype::MARKET)
    {
        is_buy=is_buy_;
        timestamp=timestamp_;
        id=id_;
        price=price_;
        quantity=quantity_;
        strcpy(venue,venue_);
        type=type_;
        strcpy(symbol,symbol_);
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

    void setType(ordertype e)
    {
       type=e;
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

    void setSymbol(const char * symbol_)
    {
        strcpy(symbol,symbol_);
    }

    void setPrice(unsigned int price_)
    {
        price=price_;
    }

    void setSide(bool is_buy_)
    {
        is_buy = is_buy_;
    }

    void setOrderID(unsigned int id_)
    {
        id=id_;

    }
    virtual unsigned int getOutstandingQuantity(){return 1;};
    virtual void setAction(unsigned int){};

};




enum order_action {add, modify, suppress};
enum message_type {none, logon, logout, new_order, execution, full_snapshot, heartbeat, incremental_snapshot};


class PriceUpdate : public Order
{
private:
    order_action action;
public:
    PriceUpdate():action(order_action::modify) {}
    order_action getAction() {return action;}
    void setAction(order_action oe) {action=oe;}
};

class Message
{
private:
    message_type type;
    Order order;
    PriceUpdate price_update;
public:
    Message():type(message_type::none),order(),price_update(){}
    void setMessageType(message_type mt){type=mt;}
    message_type getMessageType(){return type;}
    Order& getOrder() {return order;}
    PriceUpdate& getPriceUpdate() {return price_update;}

};


class Parser{
public:
    virtual bool parse(std::string message_text, Message& message){

    };
};

class Composer{
public:
    virtual std::string compose(Message& message){
        std::string return_string= "";
        return return_string;
    };
};

class FIX42Composer:public Composer{
public:
    virtual std::string compose(Message& message){
        std::string return_string= "";
        return return_string;
    };
};



class FIX42Parser : public Parser{
public:

    virtual bool parse(std::string message_text, Message& message){
        message_type mt = message_type::incremental_snapshot; // You need to get the type from message_text
        message.setMessageType(mt);
        switch (mt)
        {
            case message_type::incremental_snapshot:
                return parse_incremental_refresh(message_text,message);
            case message_type::new_order:
                return parse_new_order(message_text,message);

        }

    }
    virtual bool parse_incremental_refresh(std::string message_text, Message& message){
    };
    virtual bool parse_new_order(std::string message_text, Message& message){

    };

};


class AppBase
{
protected:
    bool is_working;
public:
    AppBase()
    {
        is_working=false;
    }
    virtual void start() = 0;
    virtual void stop() = 0;

};

class Gateway : public AppBase
{
public:
    std::map<std::string, Parser *> list_parser;
    std::map<std::string, Composer *> list_composer;
    Message stored_message;
    Gateway():stored_message(){
    }

    void start(){
        is_working = true;
    }
    void stop(){
        is_working = false;
    }

    bool add_parser(std::string venue, Parser *parser){
        list_parser[venue]=parser;
    }

    bool add_composer(std::string venue, Composer *composer){
        list_composer[venue]=composer;
    }


    bool process_message_from_exchange_for_price_update(std::string venue, std::string message_to_parse)
    {
        return list_parser[venue]->parse(message_to_parse,stored_message);
    };

    bool process_message_from_exchange_for_order(std::string venue, std::string message_to_parse)
    {
        return list_parser[venue]->parse(message_to_parse,stored_message);
    };

    std::string send_message_for_order(std::string venue,Message& me)
    {
        std::string return_string = list_composer[venue]->compose(me);

        return return_string;
    };

    Message& return_stored_message()
    {
        return stored_message;
    }

};





#endif //OEOR_HOMEWORK2_MY_CLASSES_H
