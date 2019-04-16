#include <iostream>

#include "my_unit_test.h"
#include "my_functions.h"


int main() {
    TEST_FUNCTION(my_factorial(5),120);
    TEST_FUNCTION(my_factorial(10), 3628800);
    TEST_FUNCTION(my_factorial(13), 6227020800);
    TEST_FUNCTION(my_addition(13,43), 13+43);
    TEST_FUNCTION(my_addition(4000000000,4000000000), 4000000000+4000000000);
    TEST_FUNCTION(my_power(2,16), 1<<16);
    int a[]={33,44,55,22,11};
    TEST_FUNCTION(my_average(a,5), 33);
    int b[]={3,4};
    TEST_FUNCTION(my_average(b,2), 3.5);
    TEST_FUNCTION(my_max(a,5), 55);
    TEST_FUNCTION(my_max(b,2), 4);
    int sorted_a[]={11,22,33,44,55};
    TEST_FUNCTION(is_identical_array(my_sort(a,5),sorted_a, 5, 5),true);

    PRINT_RESULTS();
    return 0;
}