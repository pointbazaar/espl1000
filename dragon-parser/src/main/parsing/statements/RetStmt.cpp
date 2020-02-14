#include <iostream>

#include "RetStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"

#include <stdio.h>

struct RetStmt* makeRetStmt(TokenList* tokens, bool debug){

	if(debug){
		cout << "RetStmt(...)" << endl;
	}

	struct RetStmt* res = (struct RetStmt*)malloc(sizeof(struct RetStmt));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	TokenList copy = TokenList(*tokens);

	copy.expect(RETURN);

	res->returnValue = makeExpr(&copy,debug);

	copy.expect(SEMICOLON);

	tokens->set(copy);

	return res;
}

