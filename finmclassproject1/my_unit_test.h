//
// Created by sebastiend on 07/09/2018.
//

#ifndef IOER_HOMEWORK1_MY_UNIT_TEST_H
#define IOER_HOMEWORK1_MY_UNIT_TEST_H

#include <iostream>

extern int number_of_tests;
extern int number_of_pass;
extern bool is_identical_array(int *array1, int *array2, int size1, int size2);

#define TEST_FUNCTION(X,Y) {number_of_tests++; if (X==Y) \
                                {std::cout << "PASS" << std::endl;number_of_pass++;} \
                            else\
                                {std::cout << "line:" << __LINE__ << \
                                " FAIL EXPECTED: " << \
                                Y << " RECEIVED: " << X << std::endl;} \
                            }
#define PRINT_RESULTS() {std::cout << "You succeeded to pass " << number_of_pass << "/" << number_of_tests;}

#endif //IOER_HOMEWORK1_MY_UNIT_TEST_H

