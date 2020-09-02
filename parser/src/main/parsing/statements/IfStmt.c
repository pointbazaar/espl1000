#include <stdio.h>
#include <stdlib.h>

#include "../StmtBlock.h"
#include "IfStmt.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../Expr.h"
#include "Stmt.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct IfStmt* makeIfStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("IfStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL; }

	struct IfStmt* res = smalloc(sizeof(struct IfStmt));
	
	res->condition = NULL;
	res->block     = NULL;
	res->elseBlock = NULL;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, IF)){
		free(res);
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

	//maybe there is an else
	struct Token* tkelse = makeToken(ELSE);
	if (list_startsWith(copy, tkelse)) {
		
		list_consume(copy, 1);

		res->elseBlock = makeStmtBlock(copy, debug);
		
		if(res->elseBlock == NULL){
			freeExpr(res->condition);
			freeStmtBlock(res->block);
			free(res);
			freeToken(tkelse);
			return NULL;
		}
	}
	freeToken(tkelse);
	
	if(debug){
		printf("sucess parsing IfStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


