#include <stdio.h>
#include <stdlib.h>

#include "LoopStmt.h"
#include "Stmt.h"
#include "../StmtBlock.h"
#include "../Expr.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../../../../ast/free_ast.h"

struct LoopStmt* makeLoopStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("LoopStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL; }

	struct LoopStmt* res = malloc(sizeof(struct LoopStmt));
	
	res->count = NULL;
	res->block     = NULL;

	struct TokenList* copy = list_copy(tokens);

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
		freeExpr(res->count);
		free(res);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing LoopStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

