#include <stdio.h>
#include <stdlib.h>

#include "AssignStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"
#include "../typenodes/Type.h"
#include "../Variable.h"
#include "../Expr.h"

struct AssignStmt* makeAssignStmt(struct TokenList* tokens,bool debug) {

	if(debug){
		printf("AssignStmt(...)\n");
	}

	struct AssignStmt* res = malloc(sizeof(struct AssignStmt));

	res->optTypeNode = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct TokenList* copy2 = list_copy(copy);
	res->optTypeNode = makeType(copy2,debug);
	if(expr != NULL){
		list_set(copy, copy2);
	}


	res->variableNode = makeVariable(copy,debug);

	if(!list_expect2(copy, OPKEY,"=")){ return NULL;}

	res->expressionNode = makeExpr(copy,debug);
	if(res->expressionNode == NULL){return NULL;}

	if(!list_expect(copy, SEMICOLON)){return NULL;}

	list_set(tokens, copy);

	return res;
}

