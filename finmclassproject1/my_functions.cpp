//
// Created by your instructor on 07/09/2018.
//

#include "my_functions.h"

// doesn't error check
long my_factorial(int number)
{
    if (number == 0)
        return 1;
    else
        return number * my_factorial(number - 1);
}

long my_addition(long number1, long number2)
{
    return number1 + number2;
}

long my_power(long number1, int number2)
{
    /* binary exponentiation
     1 in binary expansion means we use that power of base in final product */
    int res = 1;
    while (number2 != 0) {
        if (number2 & 1) {
            res *= number1;
        }
        number1 *= number1;
        number2 >>= 1;
    }
    return res;
}

double my_average(int *array_numbers, int array_size)
{
    //to access a number from a vector of ints use array_numbers[K] K
    //K is the offset where the number can be found K=[0...LIMINT_INT]
    //example array_numbers[2] will return the third element of array_numbers
    double avg;
    for (int i = 0; i < array_size; i++)
        avg += array_numbers[i];
    return avg/array_size;
}


int my_max(int *array_numbers, int array_size)
{
    int max;
    for (int i=0; i < array_size; i++)
        max = array_numbers[i] > max ? array_numbers[i] : max;
    return max;
}


int* my_sort(int *array_numbers, int array_size)
{
    // implement merge sort
    static int *head=array_numbers;
    return head;
}
