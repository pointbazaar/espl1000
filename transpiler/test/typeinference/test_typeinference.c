#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ast/util/free_ast.h>

#include "ast/ast.h"

#include "test_typeinference.h"
#include "test_typeinference_util.h"

//term/expr/const
static void test_infer_type_term();
static void test_infer_type_unopterm();
static void test_infer_type_expr();
static void test_infer_type_expr_multiple_terms();

//call
static void test_infer_type_return_type_subroutine();
static void test_infer_type_call_with_struct_member_access();
static void test_infer_type_call_with_array_access();

//variables
static void test_infer_type_simplevar_no_indices();
static void test_infer_type_simplevar_with_indices();
static void test_infer_type_var_with_member_access();

//type parameters
static void test_infer_type_type_param();
static void test_infer_type_generic_struct_member();
static void test_infer_type_generic_struct_member_nested();

static void status_test_typeinference(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_suite_typeinference(){
    test_infer_type_term();
    test_infer_type_unopterm();
    test_infer_type_expr();
    test_infer_type_expr_multiple_terms();

    //call
    test_infer_type_return_type_subroutine();
    test_infer_type_call_with_struct_member_access();
    test_infer_type_call_with_array_access();

    test_infer_type_simplevar_no_indices();
    test_infer_type_simplevar_with_indices();
    test_infer_type_var_with_member_access();

    test_infer_type_type_param();
    test_infer_type_generic_struct_member();
    test_infer_type_generic_struct_member_nested();
}

static void test_infer_type_term() {

	status_test_typeinference("infer_type_term");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_term.dg");

	assert(t != NULL);
	
	assert(t->basic_type != NULL);
	assert(t->basic_type->simple_type != NULL);
	
	assert(t->basic_type->simple_type->primitive_type != NULL);
	assert(t->basic_type->simple_type->primitive_type->is_int_type == false);
	
	assert(t->basic_type->simple_type->primitive_type->is_float_type);

	free_type(t);
}

static void test_infer_type_unopterm() {

	status_test_typeinference("infer_type_unopterm");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_unopterm.dg");

	assert(t != NULL);
	
	assert(t->basic_type != NULL);
	assert(t->basic_type->simple_type != NULL);
	
	assert(t->basic_type->simple_type->primitive_type != NULL);
	assert(t->basic_type->simple_type->primitive_type->is_int_type == false);
	
	assert(t->basic_type->simple_type->struct_type == NULL);
	
	assert(t->basic_type->simple_type->primitive_type->is_bool_type);

	free_type(t);
}

static void test_infer_type_expr() {

	status_test_typeinference("infer_type_expr");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_expr.dg");

	assert(t != NULL);
	
	assert(t->basic_type != NULL);
	assert(t->basic_type->simple_type != NULL);
	
	assert(t->basic_type->simple_type->primitive_type != NULL);
	assert(t->basic_type->simple_type->primitive_type->is_int_type);
	
	assert(t->basic_type->simple_type->struct_type == NULL);
	
	assert(t->basic_type->simple_type->primitive_type->is_int_type);

	free_type(t);
}

static void test_infer_type_expr_multiple_terms() {

	status_test_typeinference("infer_type_expr_multiple_terms");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_expr_multiple_terms.dg");

	assert(t != NULL);
	
	assert(t->basic_type != NULL);
	assert(t->basic_type->simple_type != NULL);
	
	assert(t->basic_type->simple_type->primitive_type != NULL);
	assert(t->basic_type->simple_type->primitive_type->is_int_type == false);
	
	assert(t->basic_type->simple_type->struct_type == NULL);
	
	//float + int -> float
	assert(t->basic_type->simple_type->primitive_type->is_float_type);

	free_type(t);
}

static void test_infer_type_return_type_subroutine(){

	status_test_typeinference("infer_type_return_type_subroutine");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_return_type_subroutine.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_int_type == true);

    free_type(t);
}

static void test_infer_type_call_with_struct_member_access(){

    status_test_typeinference("infer_type_call_with_struct_member_access");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_call_with_struct_member_access.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_int_type == true);

    free_type(t);
}

static void test_infer_type_call_with_array_access(){
    status_test_typeinference("infer_type_call_with_array_access");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_call_with_array_access.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_int_type == true);

    free_type(t);
}

static void test_infer_type_simplevar_no_indices(){

	status_test_typeinference("infer_type_simplevar_no_indices");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_simplevar_no_indices.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_char_type == true);

    free_type(t);
}

static void test_infer_type_simplevar_with_indices(){

	status_test_typeinference("infer_type_simplevar_with_indices");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_simplevar_with_indices.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_bool_type == true);

    free_type(t);
}

static void test_infer_type_var_with_member_access(){

	status_test_typeinference("infer_type_var_with_member_access");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_var_with_member_access.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->basic_type->simple_type != NULL);

    assert(t->basic_type->simple_type->struct_type == NULL);

    assert(t->basic_type->simple_type->primitive_type != NULL);
    assert(t->basic_type->simple_type->primitive_type->is_int_type == true);

    free_type(t);
}

static void test_infer_type_type_param(){

	status_test_typeinference("infer_type_type_param");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_type_param.dg");

    assert(t != NULL);

    assert(t->basic_type == NULL);
    assert(t->type_param != NULL);
    assert(t->array_type == NULL);

    assert(t->type_param->index == 0);

    free_type(t);
}

static void test_infer_type_generic_struct_member(){

	status_test_typeinference("infer_type_generic_struct_member");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_generic_struct_member.dg");

    assert(t != NULL);

    assert(t->basic_type != NULL);
    assert(t->type_param == NULL);
    assert(t->array_type == NULL);

    assert(t->basic_type->simple_type != NULL);
    assert(t->basic_type->simple_type->primitive_type != NULL);

    assert(!t->basic_type->simple_type->primitive_type->is_float_type);
    assert(!t->basic_type->simple_type->primitive_type->is_char_type);
    assert(!t->basic_type->simple_type->primitive_type->is_bool_type);

    assert(t->basic_type->simple_type->primitive_type->is_int_type);

    free_type(t);
}

static void test_infer_type_generic_struct_member_nested(){

	status_test_typeinference("infer_type_generic_struct_member_nested");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_generic_struct_member_nested.dg");

	assert(t != NULL);

	assert(t->basic_type != NULL);
	assert(t->type_param == NULL);
	assert(t->array_type == NULL);

	assert(t->basic_type->simple_type != NULL);
	assert(t->basic_type->simple_type->primitive_type != NULL);

	assert(!t->basic_type->simple_type->primitive_type->is_float_type);
	assert(!t->basic_type->simple_type->primitive_type->is_char_type);
	assert(!t->basic_type->simple_type->primitive_type->is_bool_type);

	assert(t->basic_type->simple_type->primitive_type->is_int_type);

	free_type(t);
}