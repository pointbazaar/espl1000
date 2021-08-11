#ifndef TESTOP
#define TESTOP

#include <stdbool.h>

#define TEST_COUNT_OP 22

/* This test suite should test that smalldragon 
 * correctly transpiles
 * all the basic operations on primitives
 */

//arithmetic tests 
void test_add();
void test_sub();
void test_mul();
void test_div();
void test_mod();

void test_precedence();

//logical tests 
void test_or();
void test_and();
void test_not();

//comparison tests 
void test_greater();
void test_lesser();
void test_geq();
void test_leq();
void test_eq();
void test_neq();
void test_chained_cmp();

//bitwise tests
void test_bitwise_and();
void test_bitwise_or();
void test_bitwise_leftshift();
void test_bitwise_rightshift();
void test_bitwise_xor();
void test_bitwise_neg();
#endif
