//
// Created by sebastiend on 21/09/2018.
//

#include "my_classes.h"

void VectorOrders::dump_list_orders(){
    printf("%s\t%s\t%s\t%s\n","Rank","ID","Price","Quantity");
    for(int k=0; k<capacity;k++)
    {
        if(orders[k]!=NULL)
            printf("%d\t%d\t%d\t%d\n",k,orders[k]->getID(), orders[k]->getPrice(),orders[k]->getQuantity());
        else
            printf("%d\tEMPTY\n",k);
    }
}

bool VectorOrders::add_order(Order *o) {
    if (current_new_order_offset >= capacity / 2) {
        double_list_orders_size();
    }
    for (int i=0; i<current_new_order_offset; i++) { // TODO: can speed this up by storing hash table of ids
        if (o->getID() == orders[i]->getID())
            return false;
    }
    orders[current_new_order_offset] = o;
    current_new_order_offset++;
    return true;
}

bool VectorOrders::delete_order(unsigned int id) {
    bool flag_removed = false;
    for (int i=0; i<current_new_order_offset; i++) {
        if (!flag_removed) {
            if (id == orders[i]->getID()) {
                flag_removed = true;
                delete orders[i];
                orders[i] = nullptr;
            }
        } else {
            orders[i-1] = orders[i];
            orders[i] = nullptr;
        }
    }
    if (flag_removed) {current_new_order_offset--;}
    return flag_removed;
}

int VectorOrders::get_total_volume() {
    int total_vol = 0;
    for (int i=0; i<current_new_order_offset; i++) {
        total_vol += orders[i]->getQuantity();
    }
    return total_vol;
}

int VectorOrders::get_total_outstanding_volume() {
    int total_vol = 0;
    for (int i=0; i<current_new_order_offset; i++) {
        total_vol += orders[i]->getOutstandingQuantity();
    }
    return total_vol;
}