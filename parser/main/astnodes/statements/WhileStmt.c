#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "WhileStmt.h"
#include "Stmt.h"
#include "expr/Expr.h"
#include "../StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct WhileStmt* makeWhileStmt(struct TokenList* tokens) {

	struct WhileStmt* res = make(WhileStmt);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));

	res->condition = NULL;
	res->block     = NULL;

	if(!list_expect(copy, WHILE)){
		//this part can be parsed deterministically
		printf("expected 'while', but was: %s\n", list_code(copy));
		exit(1);
		return NULL;
	}

	res->condition = makeExpr(copy);
	if(res->condition == NULL){
		free(res);
		return NULL;
	}

	res->block = makeStmtBlock(copy);
	if(res->block == NULL){
		free_expr(res->condition);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

