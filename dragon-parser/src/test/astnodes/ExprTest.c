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

	struct Expr* expr = makeExpr(tokens,debug);
	return (expr!=NULL)?1:0;
}

