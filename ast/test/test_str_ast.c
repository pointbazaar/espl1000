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
	
	struct BinConst* b = make(BinConst);
	
	b->value = 0b1010;
	
	char* s = strBinConst(b);
	
	assert(strcmp(s, "0b1010")==0);
	
	freeBinConst(b); free(s);
}

void test_str_intconst(){
	
	status("strIntConst");
	
	struct IntConst* b = make(IntConst);
	
	b->value = 101;
	
	char* s = strIntConst(b);
	
	assert(strcmp(s, "101")==0);
	
	freeIntConst(b); free(s);
}

void test_str_charconst(){
	
	status("strCharConst");
	
	struct CharConst* b = make(CharConst);
	
	b->value = 'h';
	
	char* s = strCharConst(b);
	
	assert(strcmp(s, "'h'")==0);
	
	freeCharConst(b); free(s);
}

void test_str_floatconst(){
	
	status("strFloatConst");
	
	struct FloatConst* b = make(FloatConst);
	
	b->value = 1.483533;
	
	char* s = strFloatConst(b);
	
	assert(strcmp(s, "1.483533")==0);
	
	freeFloatConst(b); free(s);
}

void test_str_hexconst(){
	
	status("strHexConst");
	
	struct HexConst* b = make(HexConst);
	
	b->value = 0x572af;
	
	char* s = strHexConst(b);
	
	assert(strcmp(s, "0x572af")==0);
	
	freeHexConst(b); free(s);
}


void test_str_expr(){
	
	status("strExpr");
	
	struct Op* op = make(Op);
	*op = (struct Op){
		.isArithmetic = true,
		.isRelational = false,
		.isLogical    = false,
		.isBitwise    = false
	};
	strcpy(op->op, "-");
	
	struct Term* b = make(Term);
	b->kind = 2;
	
	struct IntConst* ic = make(IntConst);
	ic->value = 45;
	
	b->ptr.m2 = ic;
	
	struct UnOpTerm* u = make(UnOpTerm);
	
	*u = (struct UnOpTerm) {
		.op   = op,
		.term = b
	};
	
	struct UnOpTerm* u2 = copyUnOpTerm(u);
	struct Op* o2       = copyOp(op);
	strcpy(o2->op, "*");
	
	struct Expr* e = make(Expr);
	
	*e = (struct Expr){
		.term1 = u,
		.op    = o2,
		.term2 = u2
	};
	
	char* s = strExpr(e);
	
	assert(strcmp(s, "-45*-45")==0);
	
	free(s);
	freeExpr(e);
}
void test_str_op(){
	
	status("strOp");
	
	struct Op* b = make(Op);
	
	strcpy(b->op, "&&");
	
	char* s = strOp(b);
	
	assert(strcmp(s, "&&")==0);
	
	freeOp(b); free(s);
}
void test_str_unopterm(){
	
	status("strUnOpTerm");
	
	struct Op* op = make(Op);
	*op = (struct Op){
		.isArithmetic = true,
		.isRelational = false,
		.isLogical    = false,
		.isBitwise    = false
	};
	strcpy(op->op, "-");
	
	struct Term* b = make(Term);
	b->kind = 2;
	
	struct IntConst* ic = make(IntConst);
	ic->value = 3489;
	
	b->ptr.m2 = ic;
	
	struct UnOpTerm* u = make(UnOpTerm);
	
	u->op   = op;
	u->term = b;
	
	char* s = strUnOpTerm(u);
	
	assert(strcmp(s, "-3489")==0);
	
	free(s);
	freeUnOpTerm(u);
}
void test_str_term(){
	
	status("strTerm");
	
	struct Term* b = make(Term);
	b->kind = 2;
	
	struct IntConst* ic = make(IntConst);
	ic->value = 3489;
	
	b->ptr.m2 = ic;
	
	char* s = strTerm(b);
	
	assert(strcmp(s, "3489")==0);
	
	freeTerm(b); free(s);
}
