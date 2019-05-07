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
#if 1
    Gateway gatewayIN;
    gatewayIN.stop();
    FIX42Parser jpmx_parser;
    FIX42Composer jpmx_composer;
    TEST_FUNCTION(gatewayIN.add_parser("JPMX",&jpmx_parser),false);
    gatewayIN.start();
    TEST_FUNCTION(gatewayIN.add_composer("JPMX",&jpmx_composer),true);
    TEST_FUNCTION(gatewayIN.add_parser("JPMX",&jpmx_parser),true);
    TEST_FUNCTION(gatewayIN.add_parser("JPMX",&jpmx_parser),false);
    std::string message1 = "8=FIX.4.2|9=95|35=X|34=5|49=VENUE|52=20160617-23:12:05.551|56=DONALD|268=1|279=1|269=1|270=110|271=5|37=9|10=209|";
    gatewayIN.stop();
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message1),false);
    gatewayIN.start();
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message1),true);

    Message msg1 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg1.getMessageType(),message_type::incremental_snapshot);
    PriceUpdate &pu1 = msg1.getPriceUpdate();
    TEST_FUNCTION(pu1.getAction(),order_action::modify);
    TEST_FUNCTION(pu1.getQuantity(),5);
    TEST_FUNCTION(pu1.getPrice(),110);
    TEST_FUNCTION(pu1.getOrderType(),ordertype::LIMIT);


    std::string message2 = "8=FIX.4.2|9=76|35=A|34=1|49=DONALD|52=20160617-23:11:55.884|56=VENUE|98=0|108=30|141=Y|10=134|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message2),true);
    Message msg2 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg2.getMessageType(),message_type::logon);

    std::string message3 = "8=FIX.4.1|9=49|35=0|34=2|49=BANZAI|52=20121105-23:24:37|56=EXEC|10=228|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message3),true);
    Message msg3 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg3.getMessageType(),message_type::heartbeat);

    std::string message4 = "8=FIX.4.2|9=54|35=5|34=20|49=VENUE|52=20160617-23:12:05.55|56=DONALD|10=134|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message4),true);
    Message msg4 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg4.getMessageType(),message_type::logout);

    std::string message5 = "8=FIX.4.2|9=207|35=W|34=2|49=VENUE|52=20160617-23:12:01.336|56=DONALD|55=MQ|268=2|269=0|270=80|271=5|37=1|269=1|270=100|271=7|37=0||37=0|10=196|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_price_update("JPMX",message5),true);
    Message msg5 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg5.getMessageType(),message_type::full_snapshot);


    Gateway gatewayOut;
    gatewayOut.start();
    gatewayOut.add_parser("JPMX",&jpmx_parser);
    gatewayOut.add_composer("JPMX",&jpmx_composer);
    std::string message6 = "8=FIX.4.2|9=124|35=D|34=2|49=DONALD|52=20160613-22:52:37.227|56=VENUE|11=1|21=3|38=3|40=2|44=100|54=1|55=MQ|60=20160613-22:52:37.227|10=163|";
    TEST_FUNCTION(gatewayOut.process_message_from_exchange_for_order("JPMX",message6),true);
    Message msg6 = gatewayOut.return_stored_message();
    TEST_FUNCTION(msg6.getMessageType(),message_type::new_order);
    Order &or1 = msg6.getOrder();
    TEST_FUNCTION(or1.getQuantity(),3);
    TEST_FUNCTION(or1.getPrice(),100);
    TEST_FUNCTION(or1.getOrderType(),ordertype::LIMIT);
    TEST_STRING(or1.getSymbol(),"MQ");
    TEST_STRING(or1.getVenue(),"JPMX");
    or1.setPrice(50);
    or1.setQuantity(111);
    TEST_STRING(gatewayOut.send_message_for_order("JPMX",msg6).c_str(),"8=FIX.4.2|9=124|35=D|34=2|49=DONALD|52=20160613-22:52:37.227|56=VENUE|11=1|21=3|38=111|40=2|44=50|54=1|55=MQ|60=20160613-22:52:37.227|10=163|");

    SEBXParser sebX_parser;
    SEBXComposer sebX_composer;
    gatewayOut.add_parser("SEBX",&sebX_parser);
    gatewayOut.add_composer("SEBX",&sebX_composer);

    std::string message7 = "NEWORDER|123|150|SEBX|MQ|";
    TEST_FUNCTION(gatewayOut.process_message_from_exchange_for_order("SEBX",message7),true);
    Message msg7 = gatewayOut.return_stored_message();
    TEST_FUNCTION(msg7.getMessageType(),message_type::new_order);
    Order &or2 = msg7.getOrder();
    TEST_FUNCTION(or2.getQuantity(),150);
    TEST_FUNCTION(or2.getPrice(),123);
    TEST_FUNCTION(or2.getOrderType(),ordertype::LIMIT);
    TEST_STRING(or2.getSymbol(),"MQ");
    TEST_STRING(or2.getVenue(),"SEBX");

    std::string message8 = "HEARTBEAT|";
    TEST_FUNCTION(gatewayOut.process_message_from_exchange_for_order("SEBX",message8),true);
    Message msg8 = gatewayOut.return_stored_message();
    TEST_FUNCTION(msg8.getMessageType(),message_type::heartbeat);

    gatewayIN.add_parser("SEBX",&sebX_parser);
    gatewayIN.add_composer("SEBX",&sebX_composer);

    std::string message9 = "LOGON|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_order("SEBX",message9),true);
    Message msg9 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg9.getMessageType(),message_type::logon);

    std::string message10 = "LOGOUT|";
    TEST_FUNCTION(gatewayIN.process_message_from_exchange_for_order("SEBX",message10),true);
    Message msg10 = gatewayIN.return_stored_message();
    TEST_FUNCTION(msg10.getMessageType(),message_type::logout);

    TEST_STRING(gatewayOut.send_message_for_order("SEBX",msg6).c_str(),"NEWORDER|111|50|SEBX|MQ|");

#endif
    PRINT_RESULTS();
    return 0;
}