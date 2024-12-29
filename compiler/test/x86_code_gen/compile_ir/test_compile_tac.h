#pragma once

void test_x86_compile_tac_nop();

void test_x86_compile_tac_goto();

void test_x86_compile_tac_copy();

void test_x86_compile_tac_const_value();

void test_x86_compile_tac_store_const_addr();

void test_x86_compile_tac_store();

void test_x86_compile_tac_load_const_addr();

void test_x86_compile_tac_load();

//TAC_UNARY_OP
void test_x86_compile_tac_unary_op_case_minus();
void test_x86_compile_tac_unary_op_case_not();
void test_x86_compile_tac_unary_op_case_bitwise_neg();

//TAC_BINARY_OP
void test_x86_compile_tac_binary_op_add_8bit();
void test_x86_compile_tac_binary_op_sub_8bit();
void test_x86_compile_tac_binary_op_and_8bit();
void test_x86_compile_tac_binary_op_or_8bit();
void test_x86_compile_tac_binary_op_xor();
void test_x86_compile_tac_binary_op_neq_true_8bit();
void test_x86_compile_tac_binary_op_neq_false_8bit();
void test_x86_compile_tac_binary_op_lt_true_8bit();
void test_x86_compile_tac_binary_op_lt_false_8bit();
void test_x86_compile_tac_binary_op_eq_true_8bit();
void test_x86_compile_tac_binary_op_eq_false_8bit();
void test_x86_compile_tac_binary_op_geq_true_8bit();
void test_x86_compile_tac_binary_op_geq_false_8bit();
void test_x86_compile_tac_binary_op_shift_left();
void test_x86_compile_tac_binary_op_shift_right();

//TAC_IF_CMP_GOTO
void test_x86_compile_tac_if_cmp_goto_case_eq_true_8bit();
void test_x86_compile_tac_if_cmp_goto_case_eq_false_8bit();
void test_x86_compile_tac_if_cmp_goto_case_neq_true_8bit();
void test_x86_compile_tac_if_cmp_goto_case_neq_false_8bit();
void test_x86_compile_tac_if_cmp_goto_case_lt_true_8bit();
void test_x86_compile_tac_if_cmp_goto_case_lt_false_8bit();
void test_x86_compile_tac_if_cmp_goto_case_ge_true_8bit();
void test_x86_compile_tac_if_cmp_goto_case_ge_false_8bit();

//TAC_IF_GOTO
void test_x86_compile_tac_if_goto_case_true();
void test_x86_compile_tac_if_goto_case_false();
void test_x86_compile_tac_if_goto_case_mixed();

void test_x86_compile_tac_load_local();

void test_x86_compile_tac_store_local();

void test_x86_compile_tac_return();

void test_x86_compile_tac_param();
void test_x86_compile_tac_call();
