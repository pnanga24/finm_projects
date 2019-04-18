//
// Created by your instructor on 07/09/2018.
//

#include "my_functions.h"
#include <assert.h>


void allocate_matrix(double **a, int rows, int columns)
{
    // You need to do the dynamic allocation on the heap
    *a = new double[rows*columns];
}

void initialize_a(double *a, int rows, int columns)
{
    //You need to initialize a
    for (int i=0; i < rows; i++) {
        for (int j=0; j < columns; j++) {
            if (i == 0)
                a[rows * i + j] = j + 1;
            else
                a[rows * i + j] = 1;
        }
    }
}

void initialize_b(double *a, int rows, int columns)
{
    assert(rows == 1);
    for (int i=0;i < columns; i++) {
        a[i] = 1;
    }
}

void initialize_c(double *a, int rows, int columns)
{
    assert(rows == 1);
    for (int i=0; i < columns; i++) {
        a[i] = 0;
    }
}


void product_sum(double *b, double (*a)[3], double *c)
{
    // computes b * a + b
    // b: 1x3, a: 3x3
    for (int i=0; i<3; i++) {
        double tmp = b[i];
        for (int j=0; j<3; j++) {
            tmp += b[j] * (*a)[j*3 + i];
        }
        c[i] = tmp;
    }
}

void product_sum_5(double *b, double (*a)[5], double *c)
{
    // computes b * a + b
    // b: 1x5, a: 5x5
    for (int i=0; i<5; i++) {
        double tmp = b[i];
        for (int j=0; j<5; j++) {
            tmp += b[j] * (*a)[j*5 + i];
        }
        c[i] = tmp;
    }
}

void display_matrix_3(double (*c)[3])
{
    for (int i=0; i<3;i++) {
        for (int k = 0; k < 3; k++)
            std::cout << c[i][k] << " ";
        std::cout << std::endl;
    }

}

void display_matrix_5(double (*c)[5])
{
    for (int i=0; i<5;i++) {
        for (int k = 0; k < 5; k++)
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