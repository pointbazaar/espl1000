#ifndef TEST_STR_AST_H
#define TEST_STR_AST_H

//const
void test_ast_str_binconst();
void test_ast_str_intconst();
void test_ast_str_charconst();
void test_ast_str_hexconst();

//expr
void test_ast_str_expr();
void test_ast_str_op();
void test_ast_str_unopterm();
void test_ast_str_term();

//struct
void test_ast_str_structdecl();
void test_ast_str_structmember();

#endif
