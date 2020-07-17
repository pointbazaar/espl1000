#include <stdio.h>
#include <stdlib.h>

#include "RetStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/Token.h"
#include "../Expr.h"

struct RetStmt* makeRetStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("RetStmt(...)\n");
	}

	struct RetStmt* res = malloc(sizeof(struct RetStmt));

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, RETURN)){return NULL;}

	res->returnValue = makeExpr(copy,debug);
	if(res->returnValue == NULL){return NULL;}

	if(!list_expect(copy, SEMICOLON)){return NULL;}
	
	if(debug){
		printf("sucess parsing RetStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeRetStmt(struct RetStmt* rs){
	
	if(rs->returnValue != NULL){
		freeExpr(rs->returnValue);
	}
	free(rs);
}
