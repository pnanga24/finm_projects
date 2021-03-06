#include <iostream>

#include "my_unit_test.h"
#include "Order.h"
#include "BookUpdate.h"
#include "MarketSimulator.h"
#include "OrderManager.h"
#include "TradingStrategy.h"
#include "MDReader.h"
#include "BookUpdate.h"
#include "BookBuilder.h"

void reset_communication(
        std::queue<Order> &strategy_to_ordermanager,
        std::queue<ExecutionOrder> &ordermanager_to_strategy,
        std::queue<Order> &ordermanager_to_simulator,
        std::queue<ExecutionOrder> &simulator_to_ordermanager
        )
{
    while (!strategy_to_ordermanager.empty()) strategy_to_ordermanager.pop();
    while (!ordermanager_to_strategy.empty()) ordermanager_to_strategy.pop();
    while (!ordermanager_to_simulator.empty()) ordermanager_to_simulator.pop();
    while (!simulator_to_ordermanager.empty()) simulator_to_ordermanager.pop();
}

int main() {
    /* Create an order o1 on the stack with the following parameter
     *
     * (100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT);
     *
     * */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

    MDReader reader("/mnt/c/Users/pnang/Documents/UChicago/Coursework/FINM/finmclassproject7/EUR_USD_Week1.csv",",",5); //MODIFY THIS LINE AND THE LINE 23 in the BookBuilder.h

    // Get the data from CSV File
    std::vector<BookUpdate> dataList = reader.getData();

    // Print the content of row by row on screen if you need
//    for(const BookUpdate &vec : dataList)
//    {
//        std::cout << vec.get_venue() << std::endl;
//        std::cout << vec.get_price() << std::endl;
//    }

    /* STEP1: You need to modify the function getData to convert a string into a epoch time */
    /* Example: https://www.epochconverter.com */

    /* Unit test will test to see if the first line of the csv file is well parsed */
    /* You will need to provide all the unit tests you need to perform this verification */
    /* ONLY FOR THE FIRST LINE */

    BookUpdate &bid = dataList[0];
    BookUpdate &ask = dataList[1];

    TEST_FUNCTION(bid.get_is_buy(), true);
    TEST_FUNCTION(bid.get_price(), 1.159500);
    TEST_FUNCTION(strcmp(bid.get_symbol(), "EUR/USD"), 0);
    TEST_FUNCTION(bid.get_timestamp(), 1535907608); // from epochconverter.com

    TEST_FUNCTION(ask.get_is_buy(), false);
    TEST_FUNCTION(ask.get_price(), 1.159800);
    TEST_FUNCTION(strcmp(ask.get_symbol(), "EUR/USD"), 0);
    TEST_FUNCTION(ask.get_timestamp(), 1535907608); // from epochconverter.com

    /* STEP2: You need to modify the class Signal to send a Signal to Long or Short based on
     * the 5-min and 20-min moving average.
     * You can create many more unit tests if necessary.
     * The goal here is to be sure that your moving average for 5 min and 20 min works
    */

    Signal sig1;
    sig1.insert_book_update(BookUpdate(0,10,10000,"GAIN",true,"EURUSD",0));
    sig1.insert_book_update(BookUpdate(0,12,10000,"GAIN",false,"EURUSD",0));
    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",280));
    sig1.insert_book_update(BookUpdate(0,15,10000,"GAIN",false,"EURUSD",280));
    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",290));
    sig1.insert_book_update(BookUpdate(0,15,10000,"GAIN",false,"EURUSD",290));
    TEST_FUNCTION(sig1.get_5min_moving_average(),13.33);
    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",300));
    sig1.insert_book_update(BookUpdate(0,16,10000,"GAIN",false,"EURUSD",300));
    TEST_FUNCTION(sig1.get_5min_moving_average(),14.67);
    TEST_FUNCTION(sig1.get_20min_moving_average(),13.75);
    sig1.insert_book_update(BookUpdate(0,16,10000,"GAIN",true,"EURUSD",1200));
    sig1.insert_book_update(BookUpdate(0,18,10000,"GAIN",false,"EURUSD",1200));
    TEST_FUNCTION(sig1.get_5min_moving_average(),17);
    TEST_FUNCTION(sig1.get_20min_moving_average(),15.25);

    /* STEP3: Generate Signal
     * if the short moving average becomes higher than the long moving average
     * you should go long so it means you will provide a signal to go long
     * if the short moving average becomes lower than the long moving average
     * you should go short so it means you will provide a signal to go short.
     * You will provide the unit tests to verify if the function
     * go_long() and go_short() works
    */

    Signal sig2;
    sig2.insert_book_update(BookUpdate(0,10,10000,"GAIN",true,"EURUSD",0));
    sig2.insert_book_update(BookUpdate(0,10,10000,"GAIN",false,"EURUSD",0));
    sig2.insert_book_update(BookUpdate(0,11,10000,"GAIN",true,"EURUSD",100));
    sig2.insert_book_update(BookUpdate(0,11,10000,"GAIN",false,"EURUSD",100));
    sig2.insert_book_update(BookUpdate(0,12,10000,"GAIN",true,"EURUSD",200));
    sig2.insert_book_update(BookUpdate(0,12,10000,"GAIN",false,"EURUSD",200));
    TEST_FUNCTION(sig2.go_long(), false);
    TEST_FUNCTION(sig2.go_short(), false);
    sig2.insert_book_update(BookUpdate(0,4,10000,"GAIN",true,"EURUSD",300));
    sig2.insert_book_update(BookUpdate(0,4,10000,"GAIN",false,"EURUSD",300));
    TEST_FUNCTION(sig2.go_short(), true);
    sig2.insert_book_update(BookUpdate(0,16,10000,"GAIN",true,"EURUSD",600));
    sig2.insert_book_update(BookUpdate(0,18,10000,"GAIN",false,"EURUSD",600));
    TEST_FUNCTION(sig2.go_long(), true);
    sig2.insert_book_update(BookUpdate(0,16,10000,"GAIN",true,"EURUSD",600));
    sig2.insert_book_update(BookUpdate(0,18,10000,"GAIN",false,"EURUSD",600));
    TEST_FUNCTION(sig2.go_long(), false);

    /* STEP4: Modify trading strategy to use the class signal correctly
     * It means that you will use go_long and go_short.
     * You will send an order buy or sell based on the return of these functions
     * You will provide the unit tests to test this part
    */

    TradingStrategy ts(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager,
                        bookbuilder_to_strategy);

    ts.start();

    for (int k=0; k<5; k++) {
        BookUpdate bu1(0, 10, 100, "BARX", true, "MSFT", 50*k);
        BookUpdate bu2(0, 10, 100, "BARX", false, "MSFT", 50*k);
        ts.process_book_update(bu1);
        ts.process_book_update(bu2);
        TEST_FUNCTION(strategy_to_ordermanager.empty(), true);
    }

    BookUpdate bu1(0, 15, 100, "BARX", true, "MSFT", 300);
    BookUpdate bu2(0, 15, 100, "BARX", false, "MSFT", 300);
    ts.process_book_update(bu1);
    ts.process_book_update(bu2);
    TEST_FUNCTION(strategy_to_ordermanager.size(), 1);
    TEST_FUNCTION(strategy_to_ordermanager.back().isBuy(), true);

    BookUpdate bu3(0, 5, 100, "BARX", true, "MSFT", 600);
    BookUpdate bu4(0, 5, 100, "BARX", false, "MSFT", 600);
    ts.process_book_update(bu3);
    ts.process_book_update(bu4);
    TEST_FUNCTION(strategy_to_ordermanager.size(), 2);
    TEST_FUNCTION(strategy_to_ordermanager.back().isBuy(), false);

    reset_communication(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);

    /* STEP5: Run your Trading Strategy in the back tester
     * In this part the Market Simulator will display the fills that your trading strategy
     * neeed. It won't reject any orders and always fill them
    */

    TradingStrategy ts1(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager,
                        bookbuilder_to_strategy);

    MarketSimulator simulator(strategy_to_ordermanager,
                              ordermanager_to_strategy,
                              ordermanager_to_simulator,
                              simulator_to_ordermanager,
                              bookbuilder_to_strategy);

    OrderManager order_manager(strategy_to_ordermanager,
                               ordermanager_to_strategy,
                               ordermanager_to_simulator,
                               simulator_to_ordermanager,
                               bookbuilder_to_strategy);

    BookBuilder book_builder(strategy_to_ordermanager,
                                ordermanager_to_strategy,
                                ordermanager_to_simulator,
                                simulator_to_ordermanager,
                                bookbuilder_to_strategy);

    /* The book builder fills the queue of BookUpdate
    * for simulation purpose
    */
    book_builder.start();
    simulator.start();
    order_manager.start();
    ts1.start();

    while(!bookbuilder_to_strategy.empty())
    {
        ts1.process_book_update();
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        order_manager.handle_execution_order();
        ts1.process_market_response();
    }




    /* STEP7: You need to modify the function get_pnl in MarketSimulator and TradingStrategy
     * to calculate the PNL or Just return the value. At the end of the execution you should the PNL
     * in both class be the same.
    */

    TEST_FUNCTION(ts1.get_pnl(),simulator.get_pnl());
    TEST_FUNCTION(lround(ts1.get_m2m_pnl() * 10) / 10.0, -125.3); // it returns -125.299..
    PRINT_RESULTS();
    return 0;
}
