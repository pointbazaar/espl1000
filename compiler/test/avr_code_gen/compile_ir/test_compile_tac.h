#pragma once

#include "libvmcu_utils/libvmcu_utils.h"

void test_compile_tac_nop();

// TAC_CONST_VALUE
void test_compile_tac_const_value_test_8bit();
void test_compile_tac_const_value_test_16bit();

// TAC_UNARY_OP
void test_compile_tac_unary_op_case_minus_8bit();
void test_compile_tac_unary_op_case_minus_16bit();
void test_compile_tac_unary_op_case_not();
void test_compile_tac_unary_op_case_bitwise_neg_8bit();
void test_compile_tac_unary_op_case_bitwise_neg_16bit();

// TAC_BINARY_OP
void test_compile_tac_binary_op_add_8bit();
void test_compile_tac_binary_op_add_16bit();
void test_compile_tac_binary_op_sub_8bit();
void test_compile_tac_binary_op_sub_16bit();
void test_compile_tac_binary_op_and_8bit();
void test_compile_tac_binary_op_and_16bit();
void test_compile_tac_binary_op_or_8bit();
void test_compile_tac_binary_op_or_16bit();
void test_compile_tac_binary_op_xor_8bit();
void test_compile_tac_binary_op_xor_16bit();
void test_compile_tac_binary_op_xor_mixed1();
void test_compile_tac_binary_op_xor_mixed2();
void test_compile_tac_binary_op_neq_true_8bit();
void test_compile_tac_binary_op_neq_true_16bit();
void test_compile_tac_binary_op_neq_false_8bit();
void test_compile_tac_binary_op_neq_false_16bit();
void test_compile_tac_binary_op_lt_true_8bit();
void test_compile_tac_binary_op_lt_true_16bit();
void test_compile_tac_binary_op_lt_false_8bit();
void test_compile_tac_binary_op_lt_false_16bit();
void test_compile_tac_binary_op_eq_true_8bit();
void test_compile_tac_binary_op_eq_true_16bit();
void test_compile_tac_binary_op_eq_false_8bit();
void test_compile_tac_binary_op_eq_false_16bit();
void test_compile_tac_binary_op_geq_true_8bit();
void test_compile_tac_binary_op_geq_true_16bit();
void test_compile_tac_binary_op_geq_false_8bit();
void test_compile_tac_binary_op_geq_false_16bit();
void test_compile_tac_binary_op_shift_left_8bit();
void test_compile_tac_binary_op_shift_left_16bit();
void test_compile_tac_binary_op_shift_right_8bit();
void test_compile_tac_binary_op_shift_right_16bit();

// TAC_GOTO
void test_compile_tac_goto();

// TAC_IF_GOTO
void test_compile_tac_if_goto_case_true_8bit();
void test_compile_tac_if_goto_case_true_16bit();
void test_compile_tac_if_goto_case_false_8bit();
void test_compile_tac_if_goto_case_false_16bit();
void test_compile_tac_if_goto_case_mixed();

// TAC_IF_CMP_GOTO
void test_compile_tac_if_cmp_goto_case_eq_true_8bit();
void test_compile_tac_if_cmp_goto_case_eq_true_16bit();
void test_compile_tac_if_cmp_goto_case_eq_false_8bit();
void test_compile_tac_if_cmp_goto_case_eq_false_16bit();
void test_compile_tac_if_cmp_goto_case_neq_true_8bit();
void test_compile_tac_if_cmp_goto_case_neq_true_16bit();
void test_compile_tac_if_cmp_goto_case_neq_false_8bit();
void test_compile_tac_if_cmp_goto_case_neq_false_16bit();
void test_compile_tac_if_cmp_goto_case_lt_true_8bit();
void test_compile_tac_if_cmp_goto_case_lt_true_16bit();
void test_compile_tac_if_cmp_goto_case_lt_false_8bit();
void test_compile_tac_if_cmp_goto_case_lt_false_16bit();
void test_compile_tac_if_cmp_goto_case_ge_true_8bit();
void test_compile_tac_if_cmp_goto_case_ge_true_16bit();
void test_compile_tac_if_cmp_goto_case_ge_false_8bit();
void test_compile_tac_if_cmp_goto_case_ge_false_16bit();

// TAC_RETURN
void test_compile_tac_return_case_8bit_0x54();
void test_compile_tac_return_case_8bit_0x55();
void test_compile_tac_return_case_16bit_0x5432();
void test_compile_tac_return_case_16bit_0x4321();

// TAC_COPY
void test_compile_tac_copy_case_8bit();
void test_compile_tac_copy_case_16bit();

// TAC_PARAM
void test_compile_tac_param_case_8bit();
void test_compile_tac_param_case_16bit();

// TAC_CALL
void test_compile_tac_call_case_recurses();
void test_compile_tac_call_case_returns_value_8bit();
void test_compile_tac_call_case_returns_value_16bit();

// TAC_LOAD
void test_compile_tac_load_case_8bit_addr_8bit_value();
void test_compile_tac_load_case_8bit_addr_16bit_value();
void test_compile_tac_load_case_16bit_addr_8bit_value();
void test_compile_tac_load_case_16bit_addr_16bit_value();

// TAC_STORE
void test_compile_tac_store_case_8bit_value_8bit_addr_c7();
void test_compile_tac_store_case_8bit_value_8bit_addr_c8();

void test_compile_tac_store_case_16bit_value_8bit_addr_c7_f03();
void test_compile_tac_store_case_16bit_value_8bit_addr_c8_f03();

void test_compile_tac_store_case_8bit_value_16bit_addr();
void test_compile_tac_store_case_16bit_value_16bit_addr();

// TAC_SETUP_SP
void test_compile_tac_setup_sp();

// TAC_SETUP_STACKFRAME
void test_compile_tac_setup_stackframe();
