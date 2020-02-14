
#include <vector>
#include <optional>
#include <iostream>

#include "AssignStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../typenodes/Type.hpp"
#include "../Variable.hpp"
#include "../Expr.hpp"

#include <stdio.h>

struct AssignStmt* makeAssignStmt(TokenList* tokens,bool debug) {

	if(debug){
		printf("AssignStmt(...)\n");
	}

	struct AssignStmt* res = (struct AssignStmt*)malloc(sizeof(struct AssignStmt));

	res->optTypeNode = NULL;

	TokenList copy = tokens->copy();

	try {
		TokenList copy2 = copy.copy();
		res->optTypeNode = makeType(&copy2,debug);
		copy.set(copy2);

	} catch (string e) {
		//pass
	}

	res->variableNode = makeVariable(&copy,debug);

	copy.expect(OPKEY,"=");

	res->expressionNode = makeExpr(&copy,debug);

	copy.expect(SEMICOLON);

	tokens->set(copy);

	return res;
}

