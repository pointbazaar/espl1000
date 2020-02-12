#include "ExprTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/Expr.hpp"

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

