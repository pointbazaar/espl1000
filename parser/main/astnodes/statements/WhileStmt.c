#include <stdio.h>
#include <stdlib.h>

#include "WhileStmt.h"
#include "Stmt.h"
#include "expr/Expr.h"
#include "../StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct WhileStmt* makeWhileStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("WhileStmt(...)\n");
	}

	struct WhileStmt* res = malloc(sizeof(struct WhileStmt));

	res->condition = NULL;
	res->block     = NULL;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, WHILE)){
		//this part can be parsed deterministically
		printf("expected 'while', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}

	res->condition = makeExpr(copy,debug);
	if(res->condition == NULL){
		free(res);
		return NULL;
	}

	res->block = makeStmtBlock(copy, debug);
	if(res->block == NULL){
		freeExpr(res->condition);
		free(res);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing WhileStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

