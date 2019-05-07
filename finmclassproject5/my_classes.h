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
    std::string FIX_rep;
public:
    Message():type(message_type::none),order(),price_update(){}
    void setMessageType(message_type mt){type=mt;}
    message_type getMessageType(){return type;}
    Order& getOrder() {return order;}
    void setOrder(Order o) {order = o;}
    PriceUpdate& getPriceUpdate() {return price_update;}
    void setPU(PriceUpdate &pu) {price_update = pu;}
    std::string getFix() {return FIX_rep;}
    void setFix(const std::string& fix) {FIX_rep = fix;}
};


class Parser{
public:
    virtual bool parse(std::string message_text, Message& message, std::string venue_){

    };
};

class Composer{
public:
    virtual std::string compose(Message& message, std::string venue_){
        std::string return_string= "";
        return return_string;
    };
};

class FIX42Composer:public Composer{
public:

    virtual void modify(std::string& message_text, const std::string& tag, const std::string& val) {
        char delim = '|';
        int start = 0;
        int end = message_text.find(delim);
        while (end != std::string::npos)
        {
            start = end + 1;
            int eq = message_text.find('=', start);
            end = message_text.find(delim, start);
            if (message_text.substr(start, eq - start) == tag) {
                message_text.replace(eq + 1, end-eq-1, val);
                return;
            }
        }
    }

    virtual std::string compose(Message& message, std::string venue_){
        std::string fix = message.getFix();
        Order o = message.getOrder();
        modify(fix, "44", std::to_string(o.getPrice()));
        modify(fix, "38", std::to_string(o.getQuantity()));
        return fix;
    };
};



class FIX42Parser : public Parser{
public:
    virtual std::string extract(const std::string& message_text, const std::string& tag) {
        char delim = '|';
        int start = 0;
        int end = message_text.find(delim);
        while (end != std::string::npos)
        {
            start = end + 1;
            int eq = message_text.find('=', start);
            end = message_text.find(delim, start);
            if (message_text.substr(start, eq - start) == tag)
                return message_text.substr(eq + 1, end - eq - 1);
        }
        return "ERROR";
    }

    virtual message_type parse_type(const std::string& message_text) {
        std::string mt = extract(message_text, "35");
        if (mt == "A")
            return message_type::logon;
        else if (mt == "D")
            return message_type::new_order;
        else if (mt == "X")
            return message_type::incremental_snapshot;
        else if (mt == "0")
            return message_type::heartbeat;
        else if (mt == "8")
            return message_type::execution;
        else if (mt == "5")
            return message_type::logout;
        else if (mt == "W")
            return message_type::full_snapshot;
        else
            return message_type::none;
    }

    virtual bool parse(std::string message_text, Message& message, std::string venue){
        message_type mt = parse_type(message_text); // You need to get the type from message_text
        message.setMessageType(mt);
        message.setFix(message_text);
        switch (mt)
        {
            case message_type::incremental_snapshot:
                return parse_incremental_refresh(message_text,message, venue);
            case message_type::new_order:
                return parse_new_order(message_text,message, venue);

        }

    }
    virtual bool parse_incremental_refresh(std::string message_text, Message& message, std::string venue_){
        PriceUpdate p;
        unsigned int price = std::atoi(extract(message_text, "270").c_str());
        unsigned int quantity = std::atoi(extract(message_text, "271").c_str());
        ordertype type = ordertype::LIMIT;
        p.setQuantity(quantity);
        p.setPrice(price);
        p.setType(type);
        p.setVenue(venue_.c_str());
        message.setPU(p);
        return true;
    };

    virtual bool parse_new_order(std::string message_text, Message& message, std::string venue_){
        bool is_buy = extract(message_text, "54") == "1";
        const char * venue = venue_.c_str();
        char symbol[20];
        strcpy(symbol, extract(message_text, "55").c_str());
        unsigned int price = std::atoi(extract(message_text, "44").c_str());
        unsigned int quantity = std::atoi(extract(message_text, "38").c_str());
        ordertype type = extract(message_text, "40") == "2" ? ordertype::LIMIT : ordertype::MARKET;
        Order o(0, is_buy, 0, price, quantity, venue, symbol, type);
        message.setOrder(o);
        return true;
    };

};

class SEBXComposer:public Composer{
public:
    virtual std::string compose(Message& message, std::string venue_){
        std::string return_string= "NEWORDER|";
        Order o = message.getOrder();
        return_string += std::to_string(o.getQuantity()) + "|";
        return_string += std::to_string(o.getPrice()) + "|";
        return_string += venue_ + "|";
        return_string += o.getSymbol();
        return_string += "|";
        return return_string;
    };
};

class SEBXParser : public Parser{
public:

    virtual message_type parse_type(const std::string& message_text) {
        std::string mt = message_text.substr(0, message_text.find("|"));
        if (mt == "LOGON")
            return message_type::logon;
        else if (mt == "NEWORDER")
            return message_type::new_order;
        else if (mt == "HEARTBEAT")
            return message_type::heartbeat;
        else if (mt == "LOGOUT")
            return message_type::logout;
        else
            return message_type::none;
    }

    virtual bool parse(std::string message_text, Message& message, std::string venue) {
        message_type mt = parse_type(message_text); // You need to get the type from message_text
        message.setMessageType(mt);
        switch (mt) {
            case message_type::new_order:
                return parse_new_order(message_text, message, venue);
        }

    }

    virtual bool parse_new_order(std::string message_text, Message& message, std::string venue_){
        char delim = '|';
        int start = 0;
        int end = message_text.find(delim);
        unsigned int price, quantity;
        char symbol[20];
        int ctr = 0;
        while (end != std::string::npos)
        {
            start = end + 1;
            end = message_text.find(delim, start);
            if (ctr == 0)
                price = std::atoi(message_text.substr(start, end - start).c_str());
            else if (ctr == 1)
                quantity = std::atoi(message_text.substr(start, end - start).c_str());
            else if (ctr == 3)
                strcpy(symbol, message_text.substr(start, end - start).c_str());
            ctr++;
        }
        Order o(0, true, 0, price, quantity, venue_.c_str(), symbol, ordertype::LIMIT);
        message.setOrder(o);
        return true;
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
        if (is_working)
            return list_parser[venue]->parse(message_to_parse,stored_message, venue);
        else
            return false;
    };

    bool process_message_from_exchange_for_order(std::string venue, std::string message_to_parse)
    {
        if (is_working)
            return list_parser[venue]->parse(message_to_parse,stored_message, venue);
        else
            return false;
    };

    std::string send_message_for_order(std::string venue,Message& me)
    {
        std::string return_string = list_composer[venue]->compose(me, venue);

        return return_string;
    };

    Message& return_stored_message()
    {
        return stored_message;
    }

};


#endif //OEOR_HOMEWORK2_MY_CLASSES_H
