#pragma once

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "libvmcu_utils/libvmcu_utils.h"

// assign stmt
void test_gen_tac_assignstmt_case_local_int_8bit();
void test_gen_tac_assignstmt_case_local_int_16bit();
void test_gen_tac_assignstmt_case_local_struct();
void test_gen_tac_assignstmt_case_local_array();

// massign stmt
void test_gen_tac_massignstmt_case_const_addr();
void test_gen_tac_massignstmt_case_variable_addr();

// mdirect
void test_gen_tac_mdirect_case_const_addr();
void test_gen_tac_mdirect_case_variable_addr();

// expr
void test_gen_tac_expr_plus();
void test_gen_tac_expr_minus();
void test_gen_tac_expr_mul();
void test_gen_tac_expr_and();
void test_gen_tac_expr_or();
void test_gen_tac_expr_shift_left();
void test_gen_tac_expr_shift_right();
void test_gen_tac_expr_xor();
void test_gen_tac_expr_lt_8bit();
void test_gen_tac_expr_lt_16bit();
void test_gen_tac_expr_gt_8bit();
void test_gen_tac_expr_gt_16bit();
void test_gen_tac_expr_eq_8bit();
void test_gen_tac_expr_eq_16bit();
void test_gen_tac_expr_neq_8bit();
void test_gen_tac_expr_neq_16bit();

// if stmt
void test_gen_tac_ifstmt_no_else_true_8bit();
void test_gen_tac_ifstmt_no_else_true_16bit();
void test_gen_tac_ifstmt_no_else_false_8bit();
void test_gen_tac_ifstmt_no_else_false_16bit();
void test_gen_tac_ifstmt_with_else_true();
void test_gen_tac_ifstmt_with_else_false();

// while stmt
void test_gen_tac_whilestmt_case_0_rounds();
void test_gen_tac_whilestmt_case_1_rounds();
void test_gen_tac_whilestmt_case_n_rounds();
void test_gen_tac_whilestmt_case_1_rounds_break();
void test_gen_tac_whilestmt_case_1_rounds_continue();

// for stmt
void test_gen_tac_forstmt_0_rounds();
void test_gen_tac_forstmt_1_rounds();
void test_gen_tac_forstmt_1_rounds_break();
void test_gen_tac_forstmt_n_rounds();

// simplevar
void test_gen_tac_simplevar_case_no_index();
void test_gen_tac_simplevar_case_1_index();
void test_gen_tac_simplevar_case_2_index();

// variable
void test_gen_tac_variable_case_no_member_access();
void test_gen_tac_variable_case_1_member_access();

// call
void test_gen_tac_call_case_no_args();
void test_gen_tac_call_case_1_args_return();
void test_gen_tac_call_case_1_args_write_8bit();
void test_gen_tac_call_case_1_args_write_16bit();
void test_gen_tac_call_case_1_args_write_3fns();
void test_gen_tac_call_case_2_args();
void test_gen_tac_call_case_frame_pointer_restored_after_call();
void test_gen_tac_call_case_frame_pointer_restored_after_call_caller_has_locals();

// struct decl
void test_gen_tac_structdecl_case_read_struct();
void test_gen_tac_structdecl_case_write_struct();
//local struct in stack frame
//void test_gen_tac_structdecl_case_read_local_struct();
//void test_gen_tac_structdecl_case_write_local_struct();

void status_test_codegen_tac(char* msg);

vmcu_system_t* prepare_vmcu_system_from_code_snippet(char* code_snippet);
