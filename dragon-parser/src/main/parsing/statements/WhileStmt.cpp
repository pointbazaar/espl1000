
#include <vector>
#include <iostream>

#include "WhileStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"
#include "Stmt.hpp"

#include <stdio.h>

using namespace std;

struct WhileStmt* makeWhileStmt(TokenList* tokens, bool debug){

	if(debug){
		printf("WhileStmt(...)\n");
	}

	struct WhileStmt* res = (struct WhileStmt*)malloc(sizeof(struct WhileStmt));

	res->statements = (struct Stmt**)malloc(sizeof(struct Stmt*)*1);
	res->count_statements = 0;

	TokenList copy = TokenList(*tokens);

	copy.expect(WHILE);

	res->condition = makeExpr(&copy,debug);

	copy.expect(LCURLY);

	Token next = copy.get(0);
	while (next.kind != RCURLY) {


		res->statements[res->count_statements] = makeStmt(&copy,debug);
		res->count_statements++;
		int newsize = res->count_statements;
		res->statements = (struct Stmt**)realloc(res->statements, sizeof(struct Stmt*) * newsize);

		next = copy.get(0);
	}

	copy.expect(RCURLY);

	tokens->set(copy);

	return res;
}

