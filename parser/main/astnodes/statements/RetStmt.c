#include <stdio.h>
#include <stdlib.h>

#include "RetStmt.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct RetStmt* makeRetStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("RetStmt(...)\n");
	}

	struct RetStmt* res = make(RetStmt);
	
	res->super.line_num    = list_head(tokens)->line_num;
	res->super.annotations = 0;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, RETURN)){
		free(res);
		return NULL;
	}

	res->returnValue = makeExpr(copy,debug);
	if(res->returnValue == NULL){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, SEMICOLON)){
		free(res);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing RetStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


