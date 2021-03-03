#ifndef TESTOP
#define TESTOP

#include <stdbool.h>

#define TEST_COUNT_OP 22

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic operations on primitives
 */

//arithmetic tests 
void test_add(bool debug);
void test_sub(bool debug);
void test_mul(bool debug);
void test_div(bool debug);
void test_mod(bool debug);

void test_precedence(bool debug);

//logical tests 
void test_or(bool debug);
void test_and(bool debug);
void test_not(bool debug);

//comparison tests 
void test_greater(bool debug);
void test_lesser(bool debug);
void test_geq(bool debug);
void test_leq(bool debug);
void test_eq(bool debug);
void test_neq(bool debug);
void test_chained_cmp(bool debug);

//bitwise tests
void test_bitwise_and(bool debug);
void test_bitwise_or(bool debug);
void test_bitwise_leftshift(bool debug);
void test_bitwise_rightshift(bool debug);
void test_bitwise_xor(bool debug);
void test_bitwise_neg(bool debug);
#endif
