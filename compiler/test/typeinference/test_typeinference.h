#pragma once

#include <stdbool.h>

//term/expr/const
void test_infer_type_term();
void test_infer_type_unopterm();
void test_infer_type_expr();
void test_infer_type_expr_multiple_terms();

//call
void test_infer_type_return_type_subroutine();
void test_infer_type_call_with_struct_member_access();
void test_infer_type_call_with_array_access();

//variables
void test_infer_type_simplevar_no_indices();
void test_infer_type_simplevar_with_indices();
void test_infer_type_var_with_member_access();

//type parameters
void test_infer_type_type_param();
