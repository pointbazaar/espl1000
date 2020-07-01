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

	struct TokenList* copy = makeTokenList(tokens);

	if(!list_expect(copy, RETURN)){return NULL;}

	res->returnValue = makeExpr(copy,debug);
	if(res->returnValue == NULL){return NULL;}

	if(!list_expect(copy, SEMICOLON)){return NULL;}

	list_set(tokens, copy);

	return res;
}

