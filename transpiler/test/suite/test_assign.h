#ifndef TESTASSIGN
#define TESTASSIGN

#include <stdbool.h>

#define TEST_COUNT_ASSIGN 5

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic assignments
 */

//tests (5)
bool test_assign(bool debug);
bool test_assign_plus(bool debug);
bool test_assign_minus(bool debug);
bool test_assign_times(bool debug);
bool test_assign_div(bool debug);

#endif
