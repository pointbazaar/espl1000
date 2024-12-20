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

	assert(strcmp(s, "-45*-45") == 0);

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
