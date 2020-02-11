#include "ExprTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/Expr.hpp"

int expr_test_simple_expression() {

	try {
		TokenList* list = new TokenList();
		list->add(INTEGER,"4");
		struct Expr* expr = makeExpr(list,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int expr_test_variable_name_expression() {

	try {
		TokenList* list = new TokenList();

		list->add(ID,"x");
		struct Expr* expr = makeExpr(list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int expr_recognize_string_constant_expression() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(STRINGCONST,"hello");

		struct Expr* expr = makeExpr(tokens,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

