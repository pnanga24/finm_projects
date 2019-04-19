#include <iostream>

#include "my_unit_test.h"
#include "my_functions.h"
#include "my_classes.h"




int main() {
    /* Create an order o1 on the stack with the following parameter
     *
     * (100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT);
     *
     * */
#if 0
    Order o1(100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT);

    TEST_FUNCTION(o1.getID(),1);
    TEST_FUNCTION(o1.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1.getPrice(),10);
    TEST_FUNCTION(o1.getQuantity(),1000);
    TEST_STRING(o1.getVenue(),"JPM");
    TEST_STRING(o1.getSymbol(),"APPL");
    TEST_FUNCTION(o1.is_valid(), true);
    o1.setVenue("\0");
    TEST_FUNCTION(o1.is_valid(), false);
    o1.setVenue("BARX");
    TEST_FUNCTION(o1.is_valid(), true);

    // Create an order book on the stack

    OrderBook b1;

    // Add an order to the book

    TEST_FUNCTION(b1.add_order(100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT),true);
    TEST_FUNCTION(b1.add_order(100,true,2,11,1000,"BARX","APPL",ordertype::MARKET),true);

    TEST_STRING(b1.getOrderBids(1)->getVenue(), "JPM");
    TEST_STRING(b1.getOrderBids(2)->getVenue(), "BARX");
    TEST_FUNCTION(b1.getOrderBids(2)->getOrderType(), ordertype::MARKET);

    TEST_FUNCTION(b1.add_order(100,false,4,11,1000,"BARX","APPL",ordertype::MARKET),true);
    TEST_FUNCTION(b1.getOrderOffers(4)->getOrderType(), ordertype::MARKET);

    TEST_FUNCTION(b1.add_order(100,false,4,11,1000,"BARX","APPL",ordertype::MARKET),false);

    TEST_FUNCTION(b1.getNumberOfBids(),2);
    TEST_FUNCTION(b1.getNumberOfOffers(),1);

    //Reset the book
    b1.clearBooks();

    TEST_FUNCTION(b1.getNumberOfBids(),0);
    TEST_FUNCTION(b1.getNumberOfOffers(),0);

    int count_offer_null=0;
    int count_bid_null=0;
    for(int k=0;k<20;k++)
    {
        count_offer_null+=(b1.getOrderOffers(k)==NULL)?1:0;
        count_bid_null+=(b1.getOrderBids(k)==NULL)?1:0;
    }
    TEST_FUNCTION(count_offer_null+count_bid_null,40)

    for(int k=0;k<20;k++)
    {
        b1.add_order(100,false,k,50+k,1000-k*10,"BARX","APPL",ordertype::LIMIT);
        b1.add_order(100,true,k,50-k,1000+k*100,"BARX","APPL",ordertype::LIMIT);
    }

    TEST_FUNCTION(b1.getNumberOfBids(),20);
    TEST_FUNCTION(b1.getNumberOfOffers(),20);

    unsigned int sum_quantity = 0;
    for(int k=0;k<20;k++)
    {
        sum_quantity+=b1.getOrderOffers(k)->getQuantity();
        sum_quantity+=b1.getOrderBids(k)->getQuantity();
    }

    TEST_FUNCTION(sum_quantity,57100);

    for(int k=0;k<20;k++)
    {
        b1.modify_order(true,k,b1.getOrderBids(k)->getQuantity()-100);
        b1.modify_order(false,k,b1.getOrderOffers(k)->getQuantity()-100);
    }

    sum_quantity = 0;
    for(int k=0;k<20;k++)
    {
        sum_quantity+=b1.getOrderOffers(k)->getQuantity();
        sum_quantity+=b1.getOrderBids(k)->getQuantity();
    }


    TEST_FUNCTION(sum_quantity,57100-100*40);

#endif

    PRINT_RESULTS();
    return 0;
}