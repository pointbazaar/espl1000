#include "ExprTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/Expr.h"

#include <stdio.h>

int expr_test_simple_expression(bool debug) {

	if(debug){
		printf("expr_test_simple_expression(...)\n");
	}

	try {
		TokenList* list = new TokenList();
		list->add(INTEGER,"4");
		struct Expr* expr = makeExpr(list,debug);

		return 1;
	}catch (string e){
		return 0;
	}
}

int expr_test_variable_name_expression(bool debug) {

	if(debug){
		printf("expr_test_variable_name_expression(...)\n");
	}

	try {
		TokenList* list = new TokenList();

		list->add(ID,"x");
		struct Expr* expr = makeExpr(list,debug);
		return 1;
	}catch (string e){
		return 0;
	}
}

int expr_recognize_2_op_expr(bool debug) {

	if(debug){
		printf("expr_recognize_2_op_expr(...)\n");
	}

	try {
		TokenList* tokens = new TokenList();

		tokens->add(INTEGER,"1");
		tokens->add(OPKEY,"+");
		tokens->add(INTEGER,"2");

		struct Expr* expr = makeExpr(tokens,debug);
		return 1;
	}catch (string e){
		return 0;
	}
}

