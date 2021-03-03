#ifndef TESTASSIGN
#define TESTASSIGN

#include <stdbool.h>

#define TEST_COUNT_ASSIGN 5

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic assignments
 */

void test_assign(bool debug);
void test_assign_plus(bool debug);
void test_assign_minus(bool debug);
void test_assign_times(bool debug);
void test_assign_div(bool debug);

#endif
