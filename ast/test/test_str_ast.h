#ifndef TEST_STR_AST_H
#define TEST_STR_AST_H

//const
void test_str_binconst();
void test_str_intconst();
void test_str_charconst();
void test_str_floatconst();
void test_str_hexconst();

//expr
void test_str_expr();
void test_str_op();
void test_str_unopterm();
void test_str_term();

//struct
void test_str_structdecl();
void test_str_structmember();

//other
void test_str_lambda();

#endif
