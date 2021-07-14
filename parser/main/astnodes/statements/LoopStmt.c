#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "LoopStmt.h"
#include "Stmt.h"
#include "../StmtBlock.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct LoopStmt* makeLoopStmt(struct TokenList* tokens, bool debug) {
	
	if(list_size(tokens) < 3){ return NULL; }

	struct LoopStmt* res = make(LoopStmt);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->count = NULL;
	res->block     = NULL;

	if(!list_expect(copy, LOOP)){
		free(res);
		return NULL;
	}

	res->count = makeExpr(copy,debug);
	
	if(res->count == NULL){
		free(res);
		return NULL;
	}

	res->block = makeStmtBlock(copy, debug);
	
	if(res->block == NULL){
		free_expr(res->count);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

