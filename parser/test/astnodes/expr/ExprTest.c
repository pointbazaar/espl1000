#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "ExprTest.h"

#include "expr/Expr.h"
#include "expr/Term.h"
#include "const/IntConst.h"
#include "var/Variable.h"
#include "var/SimpleVar.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

int expr_test_simple_expression(bool debug) {

	if(debug){
		printf("TEST: expr_test_simple_expression(...)\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	struct Expr* expr = makeExpr(list,debug);

	assert(expr != NULL);
	assert(expr->term1 != NULL);
	assert(expr->term1->term->ptr.m2->value == 4);
	
	freeTokenList(list);
	freeExpr(expr);

	return 1;
}

int expr_test_variable_name_expression(bool debug) {

	if(debug){
		printf("TEST: expr_test_variable_name_expression(...)\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	struct Expr* expr = makeExpr(list,debug);

	assert(expr != NULL);
	assert(expr->term1->term->ptr.m6 != NULL);
	assert(expr->term1->term->ptr.m6->simpleVar != NULL);
	assert(
		strcmp(
			expr->term1->term->ptr.m6->simpleVar->name, 
			"x"
		) 
		== 0
	);
	
	freeTokenList(list);
	freeExpr(expr);

	return 1;
}

int expr_recognize_2_op_expr(bool debug) {

	if(debug){
		printf("TEST: expr_recognize_2_op_expr(...)\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(INTEGER,"1"));
	list_add(tokens, makeToken2(OPKEY_ARITHMETIC,"+"));
	list_add(tokens, makeToken2(INTEGER,"2"));

	list_add(tokens, makeToken2(LCURLY,"{"));

	struct Expr* expr = makeExpr(tokens,debug);
	assert(expr != NULL);
	assert(list_size(tokens) == 1);
	
	freeTokenList(tokens);
	freeExpr(expr);

	return 1;
}

int expr_test_comparison(bool debug){

	if(debug){
		printf("TEST: expr_test_comparison\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(ID,"x"));
	list_add(l, makeToken2(OPKEY_RELATIONAL,"<"));
	list_add(l, makeToken2(INTEGER,"5"));

	list_add(l, makeToken2(LCURLY,"{"));

	struct Expr* expr = makeExpr(l,debug);
	assert(expr != NULL);

	assert(list_size(l) == 1);

	struct Term* term1 = expr->term1->term;
	assert(term1 != NULL);

	struct Term* term2 = expr->term2->term;
	assert(term2 != NULL);

	struct Op* op = expr->op;
	assert(op != NULL);

	//assert about the terms

	struct Variable* v = term1->ptr.m6;
	assert(v != NULL);

	struct IntConst* ic = term2->ptr.m2;
	assert(ic != NULL);

	assert(ic->value == 5);

	struct SimpleVar* sv = v->simpleVar;
	assert(sv != NULL);

	assert(strcmp(sv->name, "x") == 0);
	
	freeTokenList(l);
	freeExpr(expr);

	return 1;
}
