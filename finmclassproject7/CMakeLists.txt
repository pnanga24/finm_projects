cmake_minimum_required(VERSION 3.12)
project(finm_project7)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(Boost 1.65.1 REQUIRED date_time program_options thread filesystem system unit_test_framework)
IF(${Boost_FOUND})
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

add_executable(finm_project7 main.cpp my_unit_test.cpp my_unit_test.h OrderManager.cpp OrderManager.h MarketSimulator.cpp MarketSimulator.h TradingStrategy.cpp TradingStrategy.h Order.cpp Order.h BookUpdate.cpp BookUpdate.h AppBase.cpp AppBase.h MDReader.cpp MDReader.h BookBuilder.cpp BookBuilder.h)

target_link_libraries(finm_project7 ${Boost_LIBRARIES})