#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "ast/ast.h"

#include "test_typeinference.h"
#include "test_typeinference_util.h"

//term/expr/const
static void test_infer_type_term();
static void test_infer_type_unopterm();
static void test_infer_type_expr();
static void test_infer_type_expr_multiple_terms();

static void test_infer_return_type_subroutine();

//variables
static void test_infer_type_simplevar_no_indices();
static void test_infer_type_simplevar_with_indices();
static void test_infer_type_var_with_member_access();

//type parameters
static void test_infer_type_type_param();
static void test_infer_type_generic_struct_member();


static void status(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_suite_typeinference(){
    test_infer_type_term();
    test_infer_type_unopterm();
    test_infer_type_expr();
    test_infer_type_expr_multiple_terms();

    test_infer_return_type_subroutine();

    test_infer_type_simplevar_no_indices();
    test_infer_type_simplevar_with_indices();
    test_infer_type_var_with_member_access();

    test_infer_type_type_param();
    test_infer_type_generic_struct_member();
}

static void test_infer_type_term() {
	
	status("infer_type_term");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_term.dg");

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->primitive_type->is_float_type);
}

static void test_infer_type_unopterm() {
	
	status("infer_type_unopterm");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_unopterm.dg");

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	assert(t->m1->simple_type->primitive_type->is_bool_type);
}

static void test_infer_type_expr() {

	status("infer_type_expr");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_expr.dg");

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	assert(t->m1->simple_type->primitive_type->is_int_type);
}

static void test_infer_type_expr_multiple_terms() {

	status("infer_type_expr_multiple_terms");

	struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_expr_multiple_terms.dg");

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	//float + int -> float
	assert(t->m1->simple_type->primitive_type->is_float_type);
}

static void test_infer_return_type_subroutine(){

    status("infer_return_type_subroutine");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_return_type_subroutine.dg");

    assert(t != NULL);

    assert(t->m1 != NULL);
    assert(t->m1->simple_type != NULL);

    assert(t->m1->simple_type->struct_type == NULL);

    assert(t->m1->simple_type->primitive_type != NULL);
    assert(t->m1->simple_type->primitive_type->is_int_type == true);
}

static void test_infer_type_simplevar_no_indices(){

    status("infer_type_simplevar_no_indices");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_simplevar_no_indices.dg");

    assert(t != NULL);

    assert(t->m1 != NULL);
    assert(t->m1->simple_type != NULL);

    assert(t->m1->simple_type->struct_type == NULL);

    assert(t->m1->simple_type->primitive_type != NULL);
    assert(t->m1->simple_type->primitive_type->is_char_type == true);
}

static void test_infer_type_simplevar_with_indices(){

    status("infer_type_simplevar_with_indices");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_simplevar_with_indices.dg");

    assert(t != NULL);

    assert(t->m1 != NULL);
    assert(t->m1->simple_type != NULL);

    assert(t->m1->simple_type->struct_type == NULL);

    assert(t->m1->simple_type->primitive_type != NULL);
    assert(t->m1->simple_type->primitive_type->is_bool_type == true);

}

static void test_infer_type_var_with_member_access(){

    status("infer_type_var_with_member_access");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_var_with_member_access.dg");

    assert(t != NULL);

    assert(t->m1 != NULL);
    assert(t->m1->simple_type != NULL);

    assert(t->m1->simple_type->struct_type == NULL);

    assert(t->m1->simple_type->primitive_type != NULL);
    assert(t->m1->simple_type->primitive_type->is_int_type == true);
}

static void test_infer_type_type_param(){

    status("infer_type_type_param");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_type_param.dg");

    assert(t != NULL);

    assert(t->m1 == NULL);
    assert(t->m2 != NULL);
    assert(t->m3 == NULL);

    assert(t->m2->index == 0);
}

static void test_infer_type_generic_struct_member(){

    status("infer_type_generic_struct_member");

    struct Type* t = typeinfer_in_file("test/typeinference/test-src/infer_type_generic_struct_member.dg");

    assert(t != NULL);

    assert(t->m1 != NULL);
    assert(t->m2 == NULL);
    assert(t->m3 == NULL);

    assert(t->m1->simple_type != NULL);
    assert(t->m1->simple_type->primitive_type != NULL);

    assert(!t->m1->simple_type->primitive_type->is_float_type);
    assert(!t->m1->simple_type->primitive_type->is_char_type);
    assert(!t->m1->simple_type->primitive_type->is_bool_type);

    assert(t->m1->simple_type->primitive_type->is_int_type);
}