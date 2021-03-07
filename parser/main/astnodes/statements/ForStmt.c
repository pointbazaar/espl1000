#include <stdio.h>
#include <stdlib.h>

#include "ForStmt.h"
#include "Range.h"
#include "StmtBlock.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct ForStmt* makeForStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("ForStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 6){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, FOR)){
		return NULL;
	}
	
	struct Token* head = list_head(copy);
	
	if(head->kind != ID){
		freeTokenListShallow(copy);
		return NULL;
	}
	
	list_consume(copy, 1);
	
	struct ForStmt* res = make(ForStmt);
	
	//copy the index Name
	strncpy(res->indexName,	head->value_ptr, DEFAULT_STR_SIZE);
	
	if(!list_expect(copy, IN)){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	res->range = makeRange(copy, debug);
	
	if(res->range == NULL){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	res->block = makeStmtBlock(copy, debug);
	
	if(res->block == NULL){
		freeTokenListShallow(copy);
		freeRange(res->range);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

