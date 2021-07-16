#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../util/free_ast.h"
#include "../util/str_ast.h"
#include "../util/copy_ast.h"

static void status(char* msg){

	printf(" - [TEST] %s\n", msg);
}

void test_str_binconst(){
	
	status("str_bin_const");
	
	struct BinConst b = {.value = 0b1010};
	
	char* s = str_bin_const(&b);
	
	assert(strcmp(s, "0b1010")==0);
	
	free(s);
}

void test_str_intconst(){
	
	status("str_int_const");
	
	struct IntConst b = {.value=101};
	
	char* s = str_int_const(&b);
	
	assert(strcmp(s, "101")==0);
	
	free(s);
}

void test_str_charconst(){
	
	status("str_char_const");
	
	struct CharConst b = {.value='h'};
	
	char* s = str_char_const(&b);
	
	assert(strcmp(s, "'h'")==0);
	
	free(s);
}

void test_str_floatconst(){
	
	status("str_float_const");
	
	struct FloatConst b = {.value=1.483533};
	
	char* s = str_float_const(&b);
	
	assert(strcmp(s, "1.483533")==0);
	
	free(s);
}

void test_str_hexconst(){
	
	status("str_hex_const");
	
	struct HexConst b = {.value = 0x572af};
	
	char* s = str_hex_const(&b);
	
	assert(strcmp(s, "0x572af")==0);
	
	free(s);
}

void test_str_expr(){
	
	status("str_expr");
	
	struct Op o1 = {
		.is_arithmetic = true,
		.is_relational = false,
		.is_logical    = false,
		.is_bitwise    = false
	};
	strcpy(o1.op, "-");
	
	struct IntConst ic = { .value = 45 };

	struct ConstValue cv = {.ptr.m2_int_const = &ic, .kind = 2};

	struct Term b = {
		.kind = 12,
		.ptr.m12 = &cv
	};
	
	struct UnOpTerm u = {
		.op   = &o1,
		.term = &b
	};
	
	struct UnOpTerm* u2 = copy_un_op_term(&u);
	
	struct Op* o2       = copy_op(&o1);
	strcpy(o2->op, "*");
	
	struct Expr e = {
		.term1 = &u,
		.op    = o2,
		.term2 = u2
	};
	
	char* s = str_expr(&e);
	
	assert(strcmp(s, "-45*-45")==0);
	
	free(u2);
	free(o2);
	free(s);
}
void test_str_op(){
	
	status("str_op");
	
	struct Op b;
	
	strcpy(b.op, "&&");
	
	char* s = str_op(&b);
	
	assert(strcmp(s, "&&")==0);
	
	free(s);
}
void test_str_unopterm(){
	
	status("str_un_op_term");
	
	struct Op op = {
		.is_arithmetic = true,
		.is_relational = false,
		.is_logical    = false,
		.is_bitwise    = false
	};
	strcpy(op.op, "-");
	
	struct IntConst ic = { .value = 3489 };

	struct ConstValue cv = {.ptr.m2_int_const = &ic, .kind = 2};
	
	struct Term b = { 
		.kind = 12,
		.ptr.m12 = &cv
	};
	
	struct UnOpTerm u = {
		.op   = &op,
		.term = &b
	};
	
	char* s = str_un_op_term(&u);
	
	assert(strcmp(s, "-3489")==0);
	
	free(s);
}
void test_str_term(){
	
	status("str_term");
	
	struct IntConst ic = { .value = 3489 };
	struct ConstValue cv = {.ptr.m2_int_const = &ic, .kind = 2};

	struct Term b = { 
		.kind = 12,
		.ptr.m12 = &cv
	};
	
	char* s = str_term(&b);
	
	assert(strcmp(s, "3489")==0);
	
	free(s);
}

void test_str_structdecl(){
	
	//TODO
}

void test_str_structmember(){
	
	//TODO
}

void test_str_lambda(){
	
	status("str_lambda");
	
	struct Identifier i = {
		.identifier = "a"
	};
	
	struct Op o1 = {
		.is_arithmetic = true,
		.is_relational = false,
		.is_logical    = false,
		.is_bitwise    = false
	};
	strcpy(o1.op, "-");
	
	struct IntConst ic = { .value = 2 };
	struct ConstValue cv = {.ptr.m2_int_const = &ic, .kind = 2};

	struct Term b = {
		.kind = 12,
		.ptr.m12 = &cv
	};
	
	struct UnOpTerm u = {
		.op   = &o1,
		.term = &b
	};
	
	struct UnOpTerm* u2 = copy_un_op_term(&u);
	
	struct Op* o2       = copy_op(&o1);
	strcpy(o2->op, "*");
	
	struct Expr e = {
		.term1 = &u,
		.op    = o2,
		.term2 = u2
	};
	
	struct Lambda l = { 
		.params[0] = &i,
		.count_params = 1,
		.expr = &e 
	};
	
	char* s = str_lambda(&l);
	
	assert(strcmp(s, "(a) -> -2*-2") == 0);
	
	free(s);
}
