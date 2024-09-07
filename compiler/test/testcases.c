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
    test_compile_tac_const_value,

    test_compile_tac_store_const_addr,
    test_compile_tac_load_const_addr,

    test_compile_tac_binary_op_immediate,
    test_compile_tac_unary_op,
    test_compile_tac_binary_op,

    test_compile_tac_goto,
    test_compile_tac_if_goto,
    test_compile_tac_if_cmp_goto,

    test_compile_tac_return,
    test_compile_tac_copy,
    test_compile_tac_param,
    test_compile_tac_call,
    test_compile_tac_setup_stackframe,

    test_compile_tac_load,
    test_compile_tac_store,

    //test AVR Codegen as it relates to internal timers
    test_avr_code_gen_timer,
    NULL,
};

void (*tests_tac_codegen[])() = {
    test_gen_tac_mdirect,
    test_gen_tac_assignstmt,
    test_gen_tac_massignstmt,
    test_gen_tac_expr,

    test_gen_tac_ifstmt,
    test_gen_tac_whilestmt,
    test_gen_tac_forstmt,

    test_gen_tac_simplevar,
    test_gen_tac_variable,

    test_gen_tac_call,
    test_gen_tac_structdecl,
    NULL,
};

void (*tests_x86_codegen[])() = {
    test_x86_compile_tac_nop,
    test_x86_compile_tac_const_value,

    test_x86_compile_tac_store_const_addr,
    test_x86_compile_tac_load_const_addr,

    test_x86_compile_tac_binary_op_immediate,
    test_x86_compile_tac_unary_op,
    test_x86_compile_tac_binary_op,

    test_x86_compile_tac_goto,
    test_x86_compile_tac_if_goto,
    test_x86_compile_tac_if_cmp_goto,

    test_x86_compile_tac_return,
    test_x86_compile_tac_copy,
    test_x86_compile_tac_param,
    //test_x86_compile_tac_call,
    //test_x86_compile_tac_setup_stackframe,

    test_x86_compile_tac_load,
    test_x86_compile_tac_store,

    test_x86_compile_tac_load_local,
    test_x86_compile_tac_store_local,
    NULL,
};
