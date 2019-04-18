//
// Created by your instructor on 07/09/2018.
//

#ifndef IOER_HOMEWORK1_MY_FUNCTIONS_H
#define IOER_HOMEWORK1_MY_FUNCTIONS_H
#include <stdint.h>
#include <iostream>

void allocate_matrix(double **a, int rows, int columns);
void initialize_a(double *a, int rows, int columns);
void initialize_b(double *a, int rows, int columns);
void initialize_c(double *a, int rows, int columns);
void product_sum(double *b, double (*a)[3], double *c);
void product_sum_5(double *b, double (*a)[5], double *c);
void display_matrix_3(double (*c)[3]);
void display_matrix_5(double (*c)[5]);
void display_vector(double *c, int size);


#endif //IOER_HOMEWORK1_MY_FUNCTIONS_H
