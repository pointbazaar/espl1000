#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../util/free_ast.h"
#include "../util/str_ast.h"
#include "../util/copy_ast.h"
#include "../ast.h"

static void status(char* msg){

	printf(" - [TEST] %s\n", msg);
}

void test_str_binconst(){
	
	status("strBinConst");
	
	struct BinConst b = {.value = 0b1010};
	
	char* s = strBinConst(&b);
	
	assert(strcmp(s, "0b1010")==0);
	
	free(s);
}

void test_str_intconst(){
	
	status("strIntConst");
	
	struct IntConst b = {.value=101};
	
	char* s = strIntConst(&b);
	
	assert(strcmp(s, "101")==0);
	
	free(s);
}

void test_str_charconst(){
	
	status("strCharConst");
	
	struct CharConst b = {.value='h'};
	
	char* s = strCharConst(&b);
	
	assert(strcmp(s, "'h'")==0);
	
	free(s);
}

void test_str_floatconst(){
	
	status("strFloatConst");
	
	struct FloatConst b = {.value=1.483533};
	
	char* s = strFloatConst(&b);
	
	assert(strcmp(s, "1.483533")==0);
	
	free(s);
}

void test_str_hexconst(){
	
	status("strHexConst");
	
	struct HexConst b = {.value = 0x572af};
	
	char* s = strHexConst(&b);
	
	assert(strcmp(s, "0x572af")==0);
	
	free(s);
}

void test_str_expr(){
	
	status("strExpr");
	
	struct Op o1 = {
		.isArithmetic = true,
		.isRelational = false,
		.isLogical    = false,
		.isBitwise    = false
	};
	strcpy(o1.op, "-");
	
	struct IntConst ic = { .value = 45 };
	
	struct Term b = {
		.kind = 2,
		.ptr.m2 = &ic
	};
	
	struct UnOpTerm u = {
		.op   = &o1,
		.term = &b
	};
	
	struct UnOpTerm* u2 = copyUnOpTerm(&u);
	
	struct Op* o2       = copyOp(&o1);
	strcpy(o2->op, "*");
	
	struct Expr e = {
		.term1 = &u,
		.op    = o2,
		.term2 = u2
	};
	
	char* s = strExpr(&e);
	
	assert(strcmp(s, "-45*-45")==0);
	
	free(u2);
	free(o2);
	free(s);
}
void test_str_op(){
	
	status("strOp");
	
	struct Op b;
	
	strcpy(b.op, "&&");
	
	char* s = strOp(&b);
	
	assert(strcmp(s, "&&")==0);
	
	free(s);
}
void test_str_unopterm(){
	
	status("strUnOpTerm");
	
	struct Op op = {
		.isArithmetic = true,
		.isRelational = false,
		.isLogical    = false,
		.isBitwise    = false
	};
	strcpy(op.op, "-");
	
	struct IntConst ic = { .value = 3489 };
	
	struct Term b = { 
		.kind = 2,
		.ptr.m2 = &ic
	};
	
	struct UnOpTerm u = {
		.op   = &op,
		.term = &b
	};
	
	char* s = strUnOpTerm(&u);
	
	assert(strcmp(s, "-3489")==0);
	
	free(s);
}
void test_str_term(){
	
	status("strTerm");
	
	struct IntConst ic = { .value = 3489 };
	
	struct Term b = { 
		.kind = 2,
		.ptr.m2 = &ic
	};
	
	char* s = strTerm(&b);
	
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
	
	status("strLambda");
	
	struct Identifier i = {
		.identifier = "a"
	};
	
	struct Op o1 = {
		.isArithmetic = true,
		.isRelational = false,
		.isLogical    = false,
		.isBitwise    = false
	};
	strcpy(o1.op, "-");
	
	struct IntConst ic = { .value = 2 };
	
	struct Term b = {
		.kind = 2,
		.ptr.m2 = &ic
	};
	
	struct UnOpTerm u = {
		.op   = &o1,
		.term = &b
	};
	
	struct UnOpTerm* u2 = copyUnOpTerm(&u);
	
	struct Op* o2       = copyOp(&o1);
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
	
	char* s = strLambda(&l);
	
	assert(strcmp(s, "(a) -> -2*-2") == 0);
	
	free(s);
}
