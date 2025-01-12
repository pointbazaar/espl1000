#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/test/test.h"
#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"
#include "avr_code_gen/test_avr_code_gen.h"
#include "x86_code_gen/test_x86_code_gen.h"
#include "gen_tac/test_gen_tac.h"
#include "liveness/test_liveness.h"
#include "parser/test/commandline/test.h"
#include "token/test/test.h"
#include "tac/test/test.h"
#include "rat/test/test.h"
#include "ast/test/test_str_ast.h"
#include "tables/test/test.h"

void (*tests_ast[])() = {
    test_ast_str_binconst,
    test_ast_str_intconst,
    test_ast_str_charconst,
    test_ast_str_hexconst,

    test_ast_str_expr,
    test_ast_str_op,
    test_ast_str_unopterm,
    test_ast_str_term,

    NULL,
};

void (*tests_tac[])() = {
    test_tac_tostring,
    test_tac_opt_dest,

    test_tacbuffer_tostring,
    test_tacbuffer_indexof,
    test_tacbuffer_set,
    test_tacbuffer_get,
    test_tacbuffer_get_last,
    test_tacbuffer_last_dest,
    test_tacbuffer_count,
    NULL,
};

void (*tests_rat[])() = {

    test_rat_scratch_reg,
    test_rat_alloc_different_regs,
    test_rat_alloc_in_bounds,
    test_rat_can_free,
    test_rat_occupant,
    test_rat_can_free_pair,
    test_rat_occupant_pair,
    test_rat_alloc_n_regs,
    test_rat_is_wide,
    test_rat_multiple_temps_in_register,
    NULL,
};

void (*tests_token[])() = {
    test_tokenlist_no_leak,
    test_tokenlist_1element,
    test_tokenlist_copy,
    test_tokenlist_code,
    test_tokenlist_print,
    test_tokenlist_consume,
    NULL,
};

void (*tests_lexer[])() = {
    test_plus_plus_minus_minus,

    test_can_see_line_with_semicolon,
    test_can_see_line_with_operators,
    test_lexes_return_statement_favorably,
    test_lexes_other_return_statement,

    test_identifier_1,
    test_identifier_2,

    test_struct,
    test_return,

    test_string_1,

    test_typeidentifier,
    test_typeidentifier_primitive,

    test_typeparameter_1,

    test_comma,
    test_arrow,

    //tests_const.c
    test_lexes_escaped_char,
    test_anytypetoken,
    test_true,
    test_false,
    test_char,
    test_int_1,
    test_int_2,
    test_int_3,
    test_hex,
    test_binconst,

    //tests_mixed.c
    test_mixed_1,
    test_mixed_2,
    test_mixed_3,
    test_mixed_4,
    test_mixed_5,
    test_mixed_6,
    test_mixed_7,
    test_mixed_8,
    test_mixed_9,
    test_mixed_10,
    test_mixed_11,
    test_mixed_12,
    test_mixed_13,
    test_mixed_14,
    test_mixed_15,
    test_mixed_16,
    //----------------

    //tests_operators.c
    test_operators,
    test_operators_cmp,
    test_operators_logical,
    test_operators_bitwise,
    test_assign_operators,
    //----------------

    //tests_keywords.c
    test_if_else,
    test_break,
    test_for,
    test_in,
    //----------------

    //tests_comments.c
    test_single_line_comment,
    test_multi_line_comment,

    test_rangeop_1,
    test_rangeop_2,

    test_member_access,

    test_brackets,

    test_include_decl,
    NULL,
};

void (*tests_parser[])() = {
    test_parser_suite_constnodes,
    test_parser_suite_simplevar,
    test_parser_suite_var,
    test_parser_suite_term,
    test_parser_suite_expr,
    test_parser_suite_unop,
    test_parser_suite_assignstmt,
    test_parser_suite_stmts,
    test_parser_suite_method,
    test_parser_suite_struct,
    test_parser_suite_types,
    test_parser_suite_range,
    test_parser_suite_namespace,
    NULL,
};

void (*tests_liveness[])() = {
    test_liveness_1_block,
    test_liveness_2_block,

    // 'use' map
    test_liveness_use_map_tac_call,
    test_liveness_use_map_tac_param,
    test_liveness_use_map_tac_return,
    test_liveness_use_map_tac_goto,
    test_liveness_use_map_tac_if_goto,
    test_liveness_use_map_tac_if_cmp_goto,
    test_liveness_use_map_tac_load_local,
    test_liveness_use_map_tac_load_local_addr,
    test_liveness_use_map_tac_store_local,
    test_liveness_use_map_tac_load_const_addr,
    test_liveness_use_map_tac_load,
    test_liveness_use_map_tac_store,
    test_liveness_use_map_tac_binary_op,
    test_liveness_use_map_tac_unary_op,
    test_liveness_use_map_tac_copy,
    test_liveness_use_map_tac_const_value,
    test_liveness_use_map_tac_nop,

    // 'def' map
    test_liveness_def_map_tac_call,
    test_liveness_def_map_tac_param,
    test_liveness_def_map_tac_return,
    test_liveness_def_map_tac_goto,
    test_liveness_def_map_tac_if_goto,
    test_liveness_def_map_tac_if_cmp_goto,
    test_liveness_def_map_tac_load_local,
    test_liveness_def_map_tac_load_local_addr,
    test_liveness_def_map_tac_store_local,
    test_liveness_def_map_tac_load_const_addr,
    test_liveness_def_map_tac_load,
    test_liveness_def_map_tac_store,
    test_liveness_def_map_tac_binary_op,
    test_liveness_def_map_tac_unary_op,
    test_liveness_def_map_tac_copy,
    test_liveness_def_map_tac_const_value,
    test_liveness_def_map_tac_nop,

    // 'in' map
    test_liveness_in_map,

    // 'out' map
    test_liveness_out_map_tac_call,
    test_liveness_out_map_tac_param,
    test_liveness_out_map_tac_return,
    test_liveness_out_map_tac_goto,
    test_liveness_out_map_tac_if_goto,
    test_liveness_out_map_tac_if_cmp_goto,
    test_liveness_out_map_tac_copy,
    NULL,
};

void (*tests_tables[])() = {
    test_tables_lvst_ctor,
    test_tables_lvst_sizeof_type_primitive,
    test_tables_lvst_sizeof_type_other,
    test_tables_lvst_stack_frame_size_avr,
    test_tables_lvst_stack_frame_size_x86,
    test_tables_lvst_stack_frame_offset_avr_int8,
    test_tables_lvst_stack_frame_offset_avr_int64,
    test_tables_lvst_stack_frame_offset_x86_int8,
    test_tables_lvst_stack_frame_offset_x86_int64,
    test_tables_lvst_arg_index,
    NULL,
};

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

    test_compile_tac_load_const_addr_case_8bit_value_0x55,
    test_compile_tac_load_const_addr_case_8bit_value_0x56,
    test_compile_tac_load_const_addr_case_16bit_value_0x1234,
    test_compile_tac_load_const_addr_case_16bit_value_0x4321,

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
    test_compile_tac_binary_op_shift_left_8bit,
    test_compile_tac_binary_op_shift_left_16bit,
    test_compile_tac_binary_op_shift_right_8bit,
    test_compile_tac_binary_op_shift_right_16bit,

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
    test_compile_tac_return_case_8bit_0x54,
    test_compile_tac_return_case_8bit_0x55,
    test_compile_tac_return_case_16bit_0x5432,
    test_compile_tac_return_case_16bit_0x4321,

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
    test_compile_tac_load_case_8bit_addr_8bit_value,
    test_compile_tac_load_case_8bit_addr_16bit_value,
    test_compile_tac_load_case_16bit_addr_8bit_value,
    test_compile_tac_load_case_16bit_addr_16bit_value,

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

    //TAC_LOAD_CONST_ADDR
    test_x86_compile_tac_load_const_addr,

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
    test_x86_compile_tac_binary_op_shift_left,
    test_x86_compile_tac_binary_op_shift_right,

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
    test_x86_compile_tac_call,

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
