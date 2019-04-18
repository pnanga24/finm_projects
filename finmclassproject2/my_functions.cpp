//
// Created by your instructor on 07/09/2018.
//

#include "my_functions.h"


void allocate_matrix(double **a, int rows, int columns)
{
    // You need to do the dynamic allocation on the heap
}

void initialize_a(double *a, int rows, int columns)
{
//You need to initialize a
}

void initialize_b(double *a, int rows, int columns)
{
  //You need to initialize b
}

void initialize_c(double *a, int rows, int columns)
{
    //You need to initialize c
}


void product_sum(double *a, double (*b)[3], double *c)
{
    //Missing code
}

void display_matrix_3(double (*c)[3])
{
    for (int i=0; i<3;i++) {
        for (int k = 0; k < 3; k++)
            std::cout << c[i][k] << " ";
        std::cout << std::endl;
    }

}

void display_vector(double *c, int size)
{
    for (int i=0; i<size;i++) {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
}