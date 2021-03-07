#ifndef TESTASSIGN
#define TESTASSIGN

#include <stdbool.h>

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic assignments
 */

void test_assign(bool debug);
void test_assign_plus(bool debug);
void test_assign_minus(bool debug);
void test_assign_times(bool debug);
void test_assign_div(bool debug);

//bitwise assignments
void test_assign_bitwise_and(bool debug);
void test_assign_bitwise_or(bool debug);
void test_assign_bitwise_shift_left(bool debug);
void test_assign_bitwise_shift_right(bool debug);

#endif
