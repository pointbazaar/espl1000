#ifndef TESTOP
#define TESTOP

#include <stdbool.h>

#define TEST_COUNT_OP 8

bool test_add(bool debug);
bool test_sub(bool debug);
bool test_mul(bool debug);
bool test_div(bool debug);

bool test_precedence(bool debug);

//logical tests

bool test_or(bool debug);
bool test_and(bool debug);
bool test_not(bool debug);

#endif
