#pragma once

void test_typecheck_wrong_assign_primitive();
void test_typecheck_wrong_number_of_args();
void test_typecheck_wrong_type_of_arg();
void test_typecheck_wrong_return_type();
void test_typecheck_binop_type_mismatch();
void test_typecheck_subr_not_found();
void test_typecheck_impure_called_in_pure();
void test_typecheck_condition_requires_bool();
void test_typecheck_range_requires_int();
void test_typecheck_wrong_op_unop();
void test_typecheck_too_many_indices();
void test_typecheck_local_var_not_a_subroutine();
void test_typecheck_var_not_found();
void test_typecheck_index_not_found();
void test_typecheck_no_return_stmt();
void test_typecheck_all_type_errors();
