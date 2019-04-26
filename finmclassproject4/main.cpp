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

    Order o1(100,true,1,10,1000,"JPMX","EURUSD",ordertype::LIMIT);
#if 1
    TEST_FUNCTION(o1.getID(),1);
    TEST_FUNCTION(o1.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1.getPrice(),10);
    TEST_FUNCTION(o1.getQuantity(),1000);
    TEST_STRING(o1.getVenue(),"JPMX");
    TEST_STRING(o1.getSymbol(),"EURUSD");
    TEST_FUNCTION(o1.is_valid(), true);
    o1.setVenue("\0");
    TEST_FUNCTION(o1.is_valid(), false);
    o1.setVenue("BARX");
    TEST_FUNCTION(o1.is_valid(), true);

    Order *o2= new Order(101,true,2,11,1000,"BARX","EURUSD",ordertype::LIMIT);
    Order *o3= new Order(102,true,3,12,1500,"EBS","EURUSD",ordertype::LIMIT);
    Order *o4= new Order(103,true,4,13,500,"EBS","EURUSD",ordertype::LIMIT);
    Order *o5= new Order(104,false,5,14,500,"EBS","EURUSD",ordertype::LIMIT);

    VectorOrders list_orders_eurusd(20);
    list_orders_eurusd.add_order(o2);
    list_orders_eurusd.add_order(o3);
    list_orders_eurusd.add_order(o4);
    list_orders_eurusd.add_order(o5);

    TEST_FUNCTION(list_orders_eurusd.get_size(), 4);

    list_orders_eurusd.clear();

    TEST_FUNCTION(list_orders_eurusd.get_size(), 0);
    TEST_FUNCTION(list_orders_eurusd.get_capacity(), 20);

    for(int k=0;k<20;k++)
    {
        list_orders_eurusd.add_order(new Order(104+k,true,5+k,14+k,500+k,"EBS","EURUSD",ordertype::LIMIT));
    }

    TEST_FUNCTION(list_orders_eurusd.get_capacity(), 40);
    TEST_FUNCTION(list_orders_eurusd.get_size(), 20);

    TEST_FUNCTION(list_orders_eurusd.get_total_volume(), 10190);

    // list_orders_eurusd.dump_list_orders();

    for(int k=20;k<40;k++)
    {
        list_orders_eurusd.add_order(new Order(104+k,true,5+k,14+k,500+k,"EBS","EURUSD",ordertype::LIMIT));
    }

    TEST_FUNCTION(list_orders_eurusd.get_capacity(), 80);
    TEST_FUNCTION(list_orders_eurusd.get_size(), 40);

    TEST_FUNCTION(list_orders_eurusd.get_total_volume(), 20780);

    for(int k=20;k<40;k++)
    {
        list_orders_eurusd.delete_order(5+k);
    }
    TEST_FUNCTION(list_orders_eurusd.get_total_volume(), 10190);

    Order * verif_null_order=NULL;
    for(int k=20;k<40;k++)
    {
        verif_null_order=list_orders_eurusd.get_order_list()[k];
        if (verif_null_order!=NULL)
            break;
    }
    TEST_FUNCTION(verif_null_order, (Order *)NULL);

    TEST_FUNCTION(list_orders_eurusd.get_order_list()[16]->getQuantity(),516);
    list_orders_eurusd.delete_order(16);
    TEST_FUNCTION(list_orders_eurusd.get_order_list()[16]->getQuantity(),517);
    TEST_FUNCTION(list_orders_eurusd.get_order_list()[19],(Order *)NULL);

    VectorOrders l2(list_orders_eurusd);

    TEST_FUNCTION(list_orders_eurusd.get_total_volume(),9679);
    TEST_FUNCTION(l2.get_total_volume(),9679);

    TEST_FUNCTION(l2.delete_order(4),false);
    TEST_FUNCTION(l2.delete_order(7),true);


    TEST_FUNCTION(l2.get_total_volume(),9177);
    TEST_FUNCTION(list_orders_eurusd.get_total_volume(),9679);

    list_orders_eurusd.clear();

    for(int k=0;k<20;k++)
    {
        list_orders_eurusd.add_order(new ClosedOrder(104+k,true,5+k,14+k,500+k,"EBS","EURUSD",ordertype::LIMIT));
    }

    TEST_FUNCTION(list_orders_eurusd.get_total_outstanding_volume(),0);

    list_orders_eurusd.clear();

    for(int k=0;k<20;k++)
    {
        list_orders_eurusd.add_order(new OpenOrder(104+k,true,5+k,14+k,500+k,"EBS","EURUSD",ordertype::LIMIT));
    }

    TEST_FUNCTION(list_orders_eurusd.get_total_outstanding_volume(),10190);

#endif
    PRINT_RESULTS();
    return 0;
}