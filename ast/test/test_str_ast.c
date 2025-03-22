#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../util/free_ast.h"
#include "../util/str_ast.h"
#include "../util/copy_ast.h"

static void test_status_str_ast(char* msg) {

	printf(" - [TEST] %s\n", msg);
}

void test_ast_str_binconst() {

	test_status_str_ast("str_const_value - bin");

	struct ConstValue* cv = make(ConstValue);

	cv->kind = 6;
	cv->ptr.m5_hex_const = 0b1010;

	char* s = str_const_value(cv);

	assert(strcmp(s, "0b1010") == 0);

	free(s);
	free_const_value(cv);
}

void test_ast_str_intconst() {

	test_status_str_ast("str_const_value - int");

	struct ConstValue* cv = make(ConstValue);

	cv->kind = 2;
	cv->ptr.m2_int_const = 101;

	char* s = str_const_value(cv);

	assert(strcmp(s, "101") == 0);

	free(s);
	free_const_value(cv);
}

void test_ast_str_charconst() {

	test_status_str_ast("str_const_value - char");

	struct ConstValue* cv = make(ConstValue);

	cv->kind = 3;
	cv->ptr.m3_char_const = 'h';

	char* s = str_const_value(cv);

	assert(strcmp(s, "'h'") == 0);

	free(s);
	free_const_value(cv);
}

void test_ast_str_hexconst() {

	test_status_str_ast("str_const_value - hex");

	struct ConstValue* cv = make(ConstValue);

	cv->kind = 5;
	cv->ptr.m5_hex_const = 0x572af;

	char* s = str_const_value(cv);

	assert(strcmp(s, "0x572af") == 0);

	free(s);
	free_const_value(cv);
}

void test_ast_str_expr() {

	test_status_str_ast("str_expr");

	struct ConstValue cv = {.ptr.m2_int_const = 45, .kind = 2};

	struct Term b = {
	    .kind = 12,
	    .ptr.m12 = &cv};

	struct UnOpTerm u = {
	    .op = OP_MINUS,
	    .term = &b};

	struct UnOpTerm* u2 = copy_un_op_term(&u);

	struct Expr e = {
	    .term1 = &u,
	    .op = OP_MULTIPLY,
	    .term2 = u2};

	char* s = str_expr(&e);

	assert(strcmp(s, "-45 * -45") == 0);

	free_un_op_term(u2);
	free(s);
}
void test_ast_str_op() {

	test_status_str_ast("str_op");

	char* s = str_op(OP_AND);

	assert(strcmp(s, "&") == 0);

	free(s);
}
void test_ast_str_unopterm() {

	test_status_str_ast("str_un_op_term");

	struct ConstValue cv = {.ptr.m2_int_const = 3489, .kind = 2};

	struct Term b = {
	    .kind = 12,
	    .ptr.m12 = &cv};

	struct UnOpTerm u = {
	    .op = OP_MINUS,
	    .term = &b};

	char* s = str_un_op_term(&u);

	assert(strcmp(s, "-3489") == 0);

	free(s);
}
void test_ast_str_term() {

	test_status_str_ast("str_term");

	struct ConstValue cv = {.ptr.m2_int_const = 3489, .kind = 2};

	struct Term b = {
	    .kind = 12,
	    .ptr.m12 = &cv};

	char* s = str_term(&b);

	assert(strcmp(s, "3489") == 0);

	free(s);
}

void test_ast_str_structdecl() {

	test_status_str_ast("str_struct_decl");

	struct StructType stt;
	stt.type_name = "X";

	struct SimpleType st;
	st.primitive_type = NULL;
	st.struct_type = &stt;

	struct StructDecl sd;
	sd.count_members = 0;
	sd.type = &st;

	char* s = str_struct_decl(&sd);

	assert(strcmp(s, "struct X {}") == 0);

	free(s);
}

void test_ast_str_structmember() {

	test_status_str_ast("str_struct_member");

	struct StructType stt;
	stt.type_name = "X";

	struct SimpleType st;
	st.primitive_type = NULL;
	st.struct_type = &stt;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	struct Type t;
	t.basic_type = &bt;
	t.type_param = NULL;
	t.array_type = NULL;
	t.pointer_type = NULL;

	struct StructMember sm;
	sm.name = "member";
	sm.type = &t;

	char* s = str_struct_member(&sm);

	assert(strcmp(s, "X member;") == 0);

	free(s);
}

void test_ast_str_type() {

	test_status_str_ast("str_type");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	struct Type t;
	t.basic_type = &bt;
	t.type_param = NULL;
	t.array_type = NULL;
	t.pointer_type = NULL;

	char* s = str_type(&t);

	assert(strcmp(s, "int64") == 0);

	free(s);
}

void test_ast_str_basictype() {

	test_status_str_ast("str_basictype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	char* s = str_basic_type(&bt);

	assert(strcmp(s, "int64") == 0);

	free(s);
}

void test_ast_str_simpletype() {

	test_status_str_ast("str_simpletype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	char* s = str_simple_type(&st);

	assert(strcmp(s, "int64") == 0);

	free(s);
}

void test_ast_str_primitivetype() {
	test_status_str_ast("str_primitivetype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	char* s = str_primitive_type(&pt);

	assert(strcmp(s, "int64") == 0);

	free(s);
}

void test_ast_str_subrype() {
	test_status_str_ast("str_subrtype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	struct Type t;
	t.basic_type = &bt;
	t.type_param = NULL;
	t.array_type = NULL;
	t.pointer_type = NULL;

	struct SubrType subt;
	subt.count_arg_types = 0;
	subt.has_side_effects = false;
	subt.return_type = &t;

	char* s = str_subr_type(&subt);

	assert(strcmp(s, "()->int64") == 0);

	free(s);
}

void test_ast_str_structtype() {
	test_status_str_ast("str_structtype");

	struct StructType stt;
	stt.type_name = "X";

	char* s = str_struct_type(&stt);

	assert(strcmp(s, "X") == 0);

	free(s);
}

void test_ast_str_typeparam() {
	test_status_str_ast("str_typeparam");

	struct TypeParam tp;
	tp.index = 3;

	char* s = str_type_param(&tp);

	assert(strcmp(s, "?T3") == 0);

	free(s);
}

void test_ast_str_arraytype() {
	test_status_str_ast("str_arraytype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	struct Type t1;
	t1.basic_type = &bt;
	t1.type_param = NULL;
	t1.array_type = NULL;
	t1.pointer_type = NULL;

	struct ArrayType at;
	at.element_type = &t1;

	char* s = str_array_type(&at);

	assert(strcmp(s, "[int64]") == 0);

	free(s);
}

void test_ast_str_pointertype() {
	test_status_str_ast("str_pointertype");

	struct PrimitiveType pt;
	pt.is_char_type = false;
	pt.is_bool_type = false;
	pt.is_int_type = true;
	pt.int_type = INT64;

	struct SimpleType st;
	st.primitive_type = &pt;
	st.struct_type = NULL;

	struct BasicType bt;
	bt.simple_type = &st;
	bt.subr_type = NULL;

	struct Type t1;
	t1.basic_type = &bt;
	t1.type_param = NULL;
	t1.array_type = NULL;
	t1.pointer_type = NULL;

	struct PointerType ptt;
	ptt.element_type = &t1;
	char* s = str_pointer_type(&ptt);

	assert(strcmp(s, "*int64") == 0);

	free(s);
}
