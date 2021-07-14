#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "RetStmt.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct RetStmt* makeRetStmt(struct TokenList* tokens, bool debug){

	struct RetStmt* res = make(RetStmt);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));

	if(!list_expect(copy, RETURN)){
		free(res);
		return NULL;
	}

	res->return_value = makeExpr(copy, debug);
	if(res->return_value == NULL){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, SEMICOLON)){
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


