//
// Created by sebastiend on 07/09/2018.
//

#include "my_unit_test.h"

int number_of_tests = 0;
int number_of_pass = 0;

bool is_identical_array(int *array1, int *array2, int size1, int size2)
{
    // If lengths of array are not equal means
    // array are not equal
    if (size1 != size2)
        return false;
    // Linearly compare elements
    for (int i=0; i<size1; i++)
        if (array1[i] != array2[i])
            return false;
    // If all elements were same.
    return true;
}
