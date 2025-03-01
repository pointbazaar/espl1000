#pragma once

#include <stdbool.h>

// expr
int expr_test_simple_expression();
int expr_test_variable_name_expression();
int expr_recognize_2_op_expr();
int expr_test_comparison();
int expr_test_3_terms();

// mdirect
bool mdirect_test_1();

// term
int term_test_simple_term();
int term_test_variable_term();
int term_test_parentheses();
int term_test_mdirect();

// unop
int test_unop_with();
int test_unop_without();

// Deref
int test_deref();

// AddressOf
int test_address_of();
