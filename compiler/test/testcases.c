#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"
#include "avr_code_gen/test_avr_code_gen.h"
#include "x86_code_gen/test_x86_code_gen.h"
#include "gen_tac/test_gen_tac.h"

void (*tests_typeinference[])() = {
    test_infer_type_term,
    test_infer_type_unopterm,
    test_infer_type_expr,
    test_infer_type_expr_multiple_terms,
    test_infer_type_return_type_subroutine,
    test_infer_type_call_with_struct_member_access,
    test_infer_type_call_with_array_access,
    test_infer_type_simplevar_no_indices,
    test_infer_type_simplevar_with_indices,
    test_infer_type_var_with_member_access,
    test_infer_type_type_param,
    NULL,
};

void (*tests_typechecker[])() = {
    test_typecheck_wrong_assign_primitive,
    test_typecheck_wrong_number_of_args,
    test_typecheck_wrong_type_of_arg,
    test_typecheck_wrong_return_type,
    test_typecheck_binop_type_mismatch,
    test_typecheck_subr_not_found,
    test_typecheck_impure_called_in_pure,
    test_typecheck_condition_requires_bool,
    test_typecheck_range_requires_int,
    test_typecheck_wrong_op_unop,
    test_typecheck_index_not_integer_type,
    test_typecheck_too_many_indices,
    test_typecheck_local_var_not_a_subroutine,
    test_typecheck_var_not_found,

    //one test to trigger all typechecker errors, in sequence,
    //to iterate on the error messages (better dev experience)
    //and make sure multiple errors will be found
    test_typecheck_all_type_errors,
    NULL,
};

void (*tests_avr_codegen[])() = {
    test_compile_tac_setup_sp,

    test_compile_tac_nop,

    //TAC_CONST_VALUE
    test_compile_tac_const_value_test_8bit,
    test_compile_tac_const_value_test_16bit,

    test_compile_tac_store_const_addr,
    test_compile_tac_load_const_addr,

    //TAC_BINARY_OP_IMMEDIATE
    test_compile_tac_binary_op_immediate_case_add_8bit,
    test_compile_tac_binary_op_immediate_case_add_16bit,
    test_compile_tac_binary_op_immediate_case_sub_8bit,
    test_compile_tac_binary_op_immediate_case_sub_16bit,
    test_compile_tac_binary_op_immediate_case_and_8bit,
    test_compile_tac_binary_op_immediate_case_and_16bit,
    test_compile_tac_binary_op_immediate_case_or_8bit,
    test_compile_tac_binary_op_immediate_case_or_16bit,
    test_compile_tac_binary_op_immediate_case_xor_8bit,
    test_compile_tac_binary_op_immediate_case_xor_16bit,
    test_compile_tac_binary_op_immediate_case_shift_left_8bit,
    test_compile_tac_binary_op_immediate_case_shift_left_16bit,
    test_compile_tac_binary_op_immediate_case_shift_right_8bit,
    test_compile_tac_binary_op_immediate_case_shift_right_16bit,

    // TAC_UNARY_OP
    test_compile_tac_unary_op_case_minus_8bit,
    test_compile_tac_unary_op_case_minus_16bit,
    test_compile_tac_unary_op_case_not,
    test_compile_tac_unary_op_case_bitwise_neg_8bit,
    test_compile_tac_unary_op_case_bitwise_neg_16bit,

    // TAC_BINARY_OP
    test_compile_tac_binary_op_add_8bit,
    test_compile_tac_binary_op_add_16bit,
    test_compile_tac_binary_op_sub_8bit,
    test_compile_tac_binary_op_sub_16bit,
    test_compile_tac_binary_op_and_8bit,
    test_compile_tac_binary_op_and_16bit,
    test_compile_tac_binary_op_or_8bit,
    test_compile_tac_binary_op_or_16bit,
    test_compile_tac_binary_op_xor_8bit,
    test_compile_tac_binary_op_xor_16bit,
    test_compile_tac_binary_op_xor_mixed1,
    test_compile_tac_binary_op_xor_mixed2,
    test_compile_tac_binary_op_neq_true_8bit,
    test_compile_tac_binary_op_neq_true_16bit,
    test_compile_tac_binary_op_neq_false_8bit,
    test_compile_tac_binary_op_neq_false_16bit,
    test_compile_tac_binary_op_lt_true_8bit,
    test_compile_tac_binary_op_lt_true_16bit,
    test_compile_tac_binary_op_lt_false_8bit,
    test_compile_tac_binary_op_lt_false_16bit,
    test_compile_tac_binary_op_eq_true_8bit,
    test_compile_tac_binary_op_eq_true_16bit,
    test_compile_tac_binary_op_eq_false_8bit,
    test_compile_tac_binary_op_eq_false_16bit,
    test_compile_tac_binary_op_geq_true_8bit,
    test_compile_tac_binary_op_geq_true_16bit,
    test_compile_tac_binary_op_geq_false_8bit,
    test_compile_tac_binary_op_geq_false_16bit,

    // TAC_GOTO
    test_compile_tac_goto,

    // TAC_IF_GOTO
    test_compile_tac_if_goto_case_true_8bit,
    test_compile_tac_if_goto_case_true_16bit,
    test_compile_tac_if_goto_case_false_8bit,
    test_compile_tac_if_goto_case_false_16bit,
    test_compile_tac_if_goto_case_mixed,

    // TAC_IF_CMP_GOTO
    test_compile_tac_if_cmp_goto_case_eq_true_8bit,
    test_compile_tac_if_cmp_goto_case_eq_true_16bit,
    test_compile_tac_if_cmp_goto_case_eq_false_8bit,
    test_compile_tac_if_cmp_goto_case_eq_false_16bit,
    test_compile_tac_if_cmp_goto_case_neq_true_8bit,
    test_compile_tac_if_cmp_goto_case_neq_true_16bit,
    test_compile_tac_if_cmp_goto_case_neq_false_8bit,
    test_compile_tac_if_cmp_goto_case_neq_false_16bit,
    test_compile_tac_if_cmp_goto_case_lt_true_8bit,
    test_compile_tac_if_cmp_goto_case_lt_true_16bit,
    test_compile_tac_if_cmp_goto_case_lt_false_8bit,
    test_compile_tac_if_cmp_goto_case_lt_false_16bit,
    test_compile_tac_if_cmp_goto_case_ge_true_8bit,
    test_compile_tac_if_cmp_goto_case_ge_true_16bit,
    test_compile_tac_if_cmp_goto_case_ge_false_8bit,
    test_compile_tac_if_cmp_goto_case_ge_false_16bit,

    // TAC_RETURN
    test_compile_tac_return,

    // TAC_COPY
    test_compile_tac_copy_case_8bit,
    test_compile_tac_copy_case_16bit,

    // TAC_PARAM
    test_compile_tac_param_case_8bit,
    test_compile_tac_param_case_16bit,

    // TAC_CALL
    test_compile_tac_call_case_recurses,
    test_compile_tac_call_case_returns_value_8bit,
    test_compile_tac_call_case_returns_value_16bit,
    test_compile_tac_call_case_1_param,

    // TAC_SETUP_STACKFRAME
    test_compile_tac_setup_stackframe,

    // TAC_LOAD
    test_compile_tac_load_case_8bit_addr,
    test_compile_tac_load_case_16bit_addr,

    // TAC_STORE
    test_compile_tac_store_case_8bit_value_8bit_addr_c7,
    test_compile_tac_store_case_8bit_value_8bit_addr_c8,
    test_compile_tac_store_case_16bit_value_8bit_addr_c7_f03,
    test_compile_tac_store_case_16bit_value_8bit_addr_c8_f03,
    test_compile_tac_store_case_8bit_value_16bit_addr,
    test_compile_tac_store_case_16bit_value_16bit_addr,

    //test AVR Codegen as it relates to internal timers
    test_avr_code_gen_timer,
    NULL,
};

void (*tests_tac_codegen[])() = {
    // mdirect
    test_gen_tac_mdirect_case_const_addr,
    test_gen_tac_mdirect_case_variable_addr,

    // assign stmt
    test_gen_tac_assignstmt_case_local_int_8bit,
    test_gen_tac_assignstmt_case_local_int_16bit,
    test_gen_tac_assignstmt_case_local_struct,
    test_gen_tac_assignstmt_case_local_array,

    // massign stmt
    test_gen_tac_massignstmt_case_const_addr,
    test_gen_tac_massignstmt_case_variable_addr,

    // expr
    test_gen_tac_expr_plus,
    test_gen_tac_expr_minus,
    test_gen_tac_expr_mul,
    test_gen_tac_expr_and,
    test_gen_tac_expr_or,
    test_gen_tac_expr_shift_left,
    test_gen_tac_expr_shift_right,
    test_gen_tac_expr_xor,
    test_gen_tac_expr_lt_8bit,
    test_gen_tac_expr_lt_16bit,
    test_gen_tac_expr_gt_8bit,
    test_gen_tac_expr_gt_16bit,
    test_gen_tac_expr_eq_8bit,
    test_gen_tac_expr_eq_16bit,
    test_gen_tac_expr_neq_8bit,
    test_gen_tac_expr_neq_16bit,

    // if stmt
    test_gen_tac_ifstmt_no_else_true_8bit,
    test_gen_tac_ifstmt_no_else_true_16bit,
    test_gen_tac_ifstmt_no_else_false_8bit,
    test_gen_tac_ifstmt_no_else_false_16bit,
    test_gen_tac_ifstmt_with_else_true,
    test_gen_tac_ifstmt_with_else_false,

    // while stmt
    test_gen_tac_whilestmt_case_0_rounds,
    test_gen_tac_whilestmt_case_1_rounds,
    test_gen_tac_whilestmt_case_n_rounds,
    test_gen_tac_whilestmt_case_1_rounds_break,
    test_gen_tac_whilestmt_case_1_rounds_continue,

    // for stmt
    test_gen_tac_forstmt_0_rounds,
    test_gen_tac_forstmt_1_rounds,
    test_gen_tac_forstmt_1_rounds_break,
    test_gen_tac_forstmt_n_rounds,

    // simple var
    test_gen_tac_simplevar_case_no_index,
    test_gen_tac_simplevar_case_1_index,
    test_gen_tac_simplevar_case_2_index,

    // variable
    test_gen_tac_variable_case_no_member_access,
    test_gen_tac_variable_case_1_member_access,
    test_gen_tac_variable_case_2_member_access,

    // call
    test_gen_tac_call_case_no_args,
    test_gen_tac_call_case_1_args_return,
    test_gen_tac_call_case_1_args_write_8bit,
    test_gen_tac_call_case_1_args_write_16bit,
    test_gen_tac_call_case_1_args_write_3fns,
    test_gen_tac_call_case_2_args,
    test_gen_tac_call_case_frame_pointer_restored_after_call,
    test_gen_tac_call_case_frame_pointer_restored_after_call_caller_has_locals,

    // struct decl
    test_gen_tac_structdecl_case_read_struct,
    test_gen_tac_structdecl_case_write_struct,

    NULL,
};

void (*tests_x86_codegen[])() = {
    //TAC_NOP
    test_x86_compile_tac_nop,

    //TAC_CONST_VALUE
    test_x86_compile_tac_const_value,

    //TAC_STORE_CONST_ADDR
    test_x86_compile_tac_store_const_addr,

    //TAC_LOAD_CONST_ADDR
    test_x86_compile_tac_load_const_addr,

    //TAC_BINARY_OP_IMMEDIATE
    test_x86_compile_tac_binary_op_immediate_case_add,
    test_x86_compile_tac_binary_op_immediate_case_sub,
    test_x86_compile_tac_binary_op_immediate_case_and,
    test_x86_compile_tac_binary_op_immediate_case_or,
    test_x86_compile_tac_binary_op_immediate_case_xor,
    test_x86_compile_tac_binary_op_immediate_case_shift_left,
    test_x86_compile_tac_binary_op_immediate_case_shift_right,

    //TAC_UNARY_OP
    test_x86_compile_tac_unary_op_case_minus,
    test_x86_compile_tac_unary_op_case_not,
    test_x86_compile_tac_unary_op_case_bitwise_neg,

    //TAC_BINARY_OP
    test_x86_compile_tac_binary_op_add_8bit,
    test_x86_compile_tac_binary_op_sub_8bit,
    test_x86_compile_tac_binary_op_and_8bit,
    test_x86_compile_tac_binary_op_or_8bit,
    test_x86_compile_tac_binary_op_xor,
    test_x86_compile_tac_binary_op_neq_true_8bit,
    test_x86_compile_tac_binary_op_neq_false_8bit,
    test_x86_compile_tac_binary_op_lt_true_8bit,
    test_x86_compile_tac_binary_op_lt_false_8bit,
    test_x86_compile_tac_binary_op_eq_true_8bit,
    test_x86_compile_tac_binary_op_eq_false_8bit,
    test_x86_compile_tac_binary_op_geq_true_8bit,
    test_x86_compile_tac_binary_op_geq_false_8bit,

    //TAC_GOTO
    test_x86_compile_tac_goto,

    //TAC_IF_GOTO
    test_x86_compile_tac_if_goto_case_true,
    test_x86_compile_tac_if_goto_case_false,
    test_x86_compile_tac_if_goto_case_mixed,

    //TAC_IF_CMP_GOTO
    test_x86_compile_tac_if_cmp_goto_case_eq_true_8bit,
    test_x86_compile_tac_if_cmp_goto_case_eq_false_8bit,
    test_x86_compile_tac_if_cmp_goto_case_neq_true_8bit,
    test_x86_compile_tac_if_cmp_goto_case_neq_false_8bit,
    test_x86_compile_tac_if_cmp_goto_case_lt_true_8bit,
    test_x86_compile_tac_if_cmp_goto_case_lt_false_8bit,
    test_x86_compile_tac_if_cmp_goto_case_ge_true_8bit,
    test_x86_compile_tac_if_cmp_goto_case_ge_false_8bit,

    //TAC_RETURN
    test_x86_compile_tac_return,

    //TAC_COPY
    test_x86_compile_tac_copy,

    //TAC_PARAM
    test_x86_compile_tac_param,

    //TAC_CALL
    //test_x86_compile_tac_call,

    //TAC_SETUP_STACKFRAME
    //test_x86_compile_tac_setup_stackframe,

    //TAC_LOAD
    test_x86_compile_tac_load,

    //TAC_STORE
    test_x86_compile_tac_store,

    //TAC_LOAD_LOCAL
    test_x86_compile_tac_load_local,

    //TAC_STORE_LOCAL
    test_x86_compile_tac_store_local,
    NULL,
};
