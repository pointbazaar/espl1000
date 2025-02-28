#pragma once

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

//types
void test_ast_str_type();
void test_ast_str_basictype();
void test_ast_str_simpletype();
void test_ast_str_primitivetype();
void test_ast_str_subrype();
void test_ast_str_structtype();
void test_ast_str_typeparam();
void test_ast_str_arraytype();
void test_ast_str_pointertype();
