#include <iostream>

#include "my_unit_test.h"
#include "my_functions.h"




int main() {

    /* Matrix Allocation on the heap */
    double *a=NULL; //Matrix 5x5
    double *b=NULL; //Vector 1x5
    double *c=NULL; //Vector 1x5

    TEST_FUNCTION(a,(double *)NULL);
    TEST_FUNCTION(b,(double *)NULL);
    TEST_FUNCTION(c,(double *)NULL);

    allocate_matrix(&a,5,5);
    allocate_matrix(&b,1,5);
    allocate_matrix(&c,1,5);

    TEST_NOT_EQUAL(a,(double *)NULL);
    TEST_NOT_EQUAL(b,(double *)NULL);
    TEST_NOT_EQUAL(c,(double *)NULL);

    /* We want to have the following
     *
     *      1  2  3
     * a =  1  1  1
     *      1  1  1
     *
     *
     * b =  1  1  1
     *
     *
     * c =  0  0  0
    */


    initialize_a(a,3,3);
    initialize_b(b,1,3);
    initialize_c(c,1,3);

    TEST_FUNCTION(check_a(a),true);
    TEST_FUNCTION(check_b(b,3),true);
    TEST_FUNCTION(check_c(c,3),true);


/* You want to calculate
 *
 *      c= b * a + b
 *
 * You will create the function product_sum to perform this calculation
 *
*/

    product_sum(b,(double (*)[3])a,c);
    std::cout << std::endl;

    display_matrix_3((double (*)[3]) a);
    std::cout << std::endl;

    display_vector(c,3);
    TEST_FUNCTION(check_c_result_3(c),true);


    /* Strong of this experience
     * You are now going to redo the same process
     * but with matrix 5x5
    */

/* We want to have the following
     *
     *      1  2  3 4 5
     *      1  1  1 1 1
     * a =  1  1  1 1 1
     *      1  1  1 1 1
     *      1  1  1 1 1
     *
     * b =  1  1  1 1 1
     *
     *
     * c =  0  0  0 0 0
    */

/* Modify the previous functions to initialize
 * a,b and c as shown above
 */

    initialize_a(a,5,5);
    initialize_b(b,1,5);
    initialize_c(c,1,5);

// You need to modify the following functions to get the results
// For a the matrix and vectors above
// You do not need to modify the allocation since the allocation
// Already works for 5x5 and 1x5 and 1x5
// You will certainly need to create another function product_sum_5 based on product_sum to make it work

    product_sum_5(b,(double (*)[5])a,c);
    std::cout << std::endl;

// You need to display the result with the function display_matrix_5
//  that you will create based on display_matrix_3

    display_matrix_5((double (*)[5]) a);
    std::cout << std::endl;

    display_vector(c,5);
    TEST_FUNCTION(check_c_result_5(c),true);
// You should see displayed:
// You succeeded to pass 11/11

PRINT_RESULTS();
return 0;
}