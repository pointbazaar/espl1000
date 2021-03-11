#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StmtBlock.h"
#include "statements/Stmt.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct StmtBlock* makeStmtBlock(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("makeStmtBlock from:");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 2){ return NULL; }
	
	struct StmtBlock* res = make(StmtBlock);
	
	res->count = 0;
	res->stmts = malloc(sizeof(struct Stmt*)*1);
	
	struct TokenList* copy = list_copy(tokens);
	
	if(!list_expect(copy, LCURLY)){
		freeTokenListShallow(copy);
		free(res->stmts);
		free(res);
		return NULL;
	}

	struct Token* next = list_head(copy);
	
	while (next->kind != RCURLY) {

		struct Stmt* stmt = makeStmt(copy, debug);
		
		if(stmt == NULL){
			freeTokenListShallow(copy);
			free(res->stmts);
			free(res);
			return NULL;
		}

		res->stmts[res->count] = stmt;
		res->count++;

		const int newsize = sizeof(struct Stmt*) * (res->count + 1);
		res->stmts = realloc(res->stmts,newsize);

		next = list_head(copy);
		if(next == NULL){
			freeTokenListShallow(copy);
			free(res->stmts);
			free(res);
			return NULL;
		}
	}

	if(!list_expect(copy, RCURLY)){
		freeTokenListShallow(copy);
		freeStmtBlock(res);
		return NULL;
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}


