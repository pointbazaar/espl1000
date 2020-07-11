#include <stdio.h>
#include <stdlib.h>

#include "AssignStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"
#include "../typenodes/Type.h"
#include "../Variable.h"
#include "../Expr.h"

struct AssignStmt* makeAssignStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("AssignStmt(...) from: %s\n", list_code(tokens, debug));
	}

	struct AssignStmt* res = malloc(sizeof(struct AssignStmt));
	if(res == NULL){return NULL;}

	res->optTypeNode = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct TokenList* copy2 = list_copy(copy);

	res->optTypeNode = makeType2(copy2,debug);
	if(res->optTypeNode != NULL){
		list_set(copy, copy2);
	}

	if(debug){
		printf("%s\n", list_code(copy, debug));
		printf("DEBUG\n");
	}

	res->variableNode = makeVariable(copy,debug);
	if(res->variableNode == NULL){return NULL;}

	if(!list_expect_2(copy, makeToken2(EQ,"="))){ return NULL;}

	res->expressionNode = makeExpr(copy,debug);
	if(res->expressionNode == NULL){return NULL;}

	if(!list_expect(copy, SEMICOLON)){return NULL;}
	
	if(debug){
		printf("sucess parsing AssignStmt\n");
	}

	list_set(tokens, copy);

	return res;
}

