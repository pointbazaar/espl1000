#include "ExprTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Expr.h"
#include "../../main/parsing/Term.h"
#include "../../main/parsing/IntConst.h"
#include "../../main/parsing/Variable.h"
#include "../../main/parsing/SimpleVar.h"

#include <stdio.h>
#include <stdbool.h>

int expr_test_simple_expression(bool debug) {

	if(debug){
		printf("expr_test_simple_expression(...)\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	struct Expr* expr = makeExpr(list,debug);

	if(expr == NULL){return 0;}

	if(expr->term1 == NULL){return 0;}

	bool a1 = expr->term1->m2->value == 4;

	return (a1)?1:0;
}

int expr_test_variable_name_expression(bool debug) {

	if(debug){
		printf("expr_test_variable_name_expression(...)\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	struct Expr* expr = makeExpr(list,debug);

	if(expr == NULL){return 0;}

	if(expr->term1->m6 == NULL){return 0;}

	if(expr->term1->m6->simpleVariableNode == NULL){return 0;}

	bool a1 = strcmp(expr->term1->m6->simpleVariableNode->name, "x") == 0;

	return (a1)?1:0;
}

int expr_recognize_2_op_expr(bool debug) {

	if(debug){
		printf("expr_recognize_2_op_expr(...)\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(INTEGER,"1"));
	list_add(tokens, makeToken2(OPKEY,"+"));
	list_add(tokens, makeToken2(INTEGER,"2"));

	list_add(tokens, makeToken2(LCURLY,"{"));

	struct Expr* expr = makeExpr(tokens,debug);
	if(expr == NULL){return 0;}

	bool a1 = list_size(tokens) == 1;

	return (a1)?1:0;
}

int expr_test_comparison(bool debug){

	if(debug){
		printf("expr_test_comparison\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(ID,"x"));
	list_add(l, makeToken2(OPKEY,"<"));
	list_add(l, makeToken2(INTEGER,"5"));

	list_add(l, makeToken2(LCURLY,"{"));

	struct Expr* expr = makeExpr(l,debug);
	if(expr == NULL){return 0;}

	bool a1 = list_size(l) == 1;

	struct Term* term1 = expr->term1;
	if(term1 == NULL){return 0;}

	struct Term* term2 = expr->term2;
	if(term2 == NULL){return 0;}

	struct Op* op = expr->op;
	if(op == NULL){return 0;}

	//assert about the terms

	struct Variable* v = term1->m6;
	if(v == NULL){return 0;}

	struct IntConst* ic = term2->m2;
	if(ic == NULL){return 0;}

	bool a2 = ic->value == 5;

	struct SimpleVar* sv = v->simpleVariableNode;
	if(sv == NULL){return 0;}

	bool a3 = strcmp(sv->name, "x") == 0;

	return (a1 && a2 && a3)?1:0;
}