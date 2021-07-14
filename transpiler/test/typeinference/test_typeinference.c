#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "ast/ast.h"


#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "transpiler/main/typeinference/typeinfer.h"

#include "test_typeinference.h"

static void status(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_infer_type_term(bool debug){
	
	status("infer_type_term");
	
	struct ST* st = makeST(debug);
	
	struct Term* term = make(Term);
	struct FloatConst* c = make(FloatConst);
	
	c->value = 83.0;
	
	term->kind = 7;
	term->ptr.m7 = c;
	
	
	struct Type* t = infer_type_term("test", st, term);

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->primitive_type->is_float_type);
}

void test_infer_type_unopterm(bool debug){
	
	status("infer_type_unopterm");
	
	struct ST* st = makeST(debug);
	
	struct Term* term = make(Term);
	struct BoolConst* c = make(BoolConst);
	struct UnOpTerm* uopt = make(UnOpTerm);
	
	c->value = false;
	
	term->kind = 1;
	term->ptr.m1 = c;
	
	uopt->op = NULL;
	uopt->term = term;

	struct Type* t = infer_type_unopterm("test", st, uopt);

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	assert(t->m1->simple_type->primitive_type->is_bool_type);
}

void test_infer_type_expr(bool debug){

	status("infer_type_expr");

	struct ST* st = makeST(debug);
	
	struct Expr* expr = make(Expr);
	struct Term* term = make(Term);
	struct IntConst* ic = make(IntConst);
	struct UnOpTerm* uopt = make(UnOpTerm);
	
	ic->value = 3;
	
	term->kind = 2;
	term->ptr.m2 = ic;
	
	uopt->op = NULL;
	uopt->term = term;
	
	expr->term1 = uopt;
	expr->op    = NULL;
	expr->term2 = NULL;

	struct Type* t = infer_type_expr("test", st, expr);

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	assert(t->m1->simple_type->primitive_type->is_int_type);
}

void test_infer_type_expr_multiple_terms(bool debug){

	status("infer_type_expr_multiple_terms");

	struct ST* st = makeST(debug);
	
	struct Expr* expr = make(Expr);
	
	struct Term* term1 = make(Term);
	struct Term* term2 = make(Term);
	
	struct IntConst* c1 = make(IntConst);
	struct FloatConst* c2 = make(FloatConst);

	struct UnOpTerm* uopt1 = make(UnOpTerm);
	struct UnOpTerm* uopt2 = make(UnOpTerm);

	struct Op* myop = make(Op);
	strcpy(myop->op, "+");
	myop->is_arithmetic = true;

	c1->value = 3;
	c2->value = 3.0;
	
	term1->kind = 2;
	term2->ptr.m2 = c1;
	
	term2->kind = 7;
	term2->ptr.m7 = c2;
	
	uopt1->op = NULL;
	uopt1->term = term1;
	
	uopt2->op = NULL;
	uopt2->term = term2;
	
	expr->term1 = uopt1;
	expr->op    = myop;
	expr->term2 = uopt2;

	struct Type* t = infer_type_expr("test", st, expr);

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simple_type != NULL);
	
	assert(t->m1->simple_type->primitive_type != NULL);
	assert(t->m1->simple_type->primitive_type->is_int_type == false);
	
	assert(t->m1->simple_type->struct_type == NULL);
	
	//float + int -> float
	assert(t->m1->simple_type->primitive_type->is_float_type);
}
