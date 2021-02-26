#ifndef TESTOP
#define TESTOP

#include <stdbool.h>

#define TEST_COUNT_OP 22

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic operations on primitives
 */

//arithmetic tests (6)
bool test_add(bool debug);
bool test_sub(bool debug);
bool test_mul(bool debug);
bool test_div(bool debug);
bool test_mod(bool debug);

bool test_precedence(bool debug);

//logical tests (3)
bool test_or(bool debug);
bool test_and(bool debug);
bool test_not(bool debug);

//comparison tests (7)
bool test_greater(bool debug);
bool test_lesser(bool debug);
bool test_geq(bool debug);
bool test_leq(bool debug);
bool test_eq(bool debug);
bool test_neq(bool debug);
bool test_chained_cmp(bool debug);

//bitwise tests (6)
bool test_bitwise_and(bool debug);
bool test_bitwise_or(bool debug);
bool test_bitwise_leftshift(bool debug);
bool test_bitwise_rightshift(bool debug);
bool test_bitwise_xor(bool debug);
bool test_bitwise_neg(bool debug);
#endif