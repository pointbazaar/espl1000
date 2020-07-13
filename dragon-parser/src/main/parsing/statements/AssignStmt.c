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

	res->optType = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct TokenList* copy2 = list_copy(copy);

	res->optType = makeType2(copy2,debug);
	if(res->optType != NULL){
		list_set(copy, copy2);
	}

	if(debug){
		printf("%s\n", list_code(copy, debug));
		printf("DEBUG\n");
	}

	res->var = makeVariable(copy,debug);
	if(res->var == NULL){return NULL;}

	if(!list_expect_2(copy, makeToken2(EQ,"="))){ return NULL;}

	res->expr = makeExpr(copy,debug);
	if(res->expr == NULL){return NULL;}

	if(!list_expect(copy, SEMICOLON)){return NULL;}
	
	if(debug){
		printf("sucess parsing AssignStmt\n");
	}

	list_set(tokens, copy);

	return res;
}

