#ifndef TESTASSIGN
#define TESTASSIGN

#include <stdbool.h>

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic assignments
 */

void test_assign();
void test_assign_plus();
void test_assign_minus();
void test_assign_times();
void test_assign_div();

//bitwise assignments
void test_assign_bitwise_and();
void test_assign_bitwise_or();
void test_assign_bitwise_shift_left();
void test_assign_bitwise_shift_right();

#endif
