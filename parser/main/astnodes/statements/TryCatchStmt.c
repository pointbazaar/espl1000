#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser/main/util/parse_astnode.h"

#include "../StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct TryCatchStmt* makeTryCatchStmt(struct TokenList* tokens, bool debug){
	
	if (list_size(tokens) == 0) { return NULL; }

	struct TryCatchStmt* res = make(TryCatchStmt);
	
	res->try_block   = NULL;
	res->catch_block = NULL;
	
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	if(!list_expect(copy, TRY)){
		printf("[Parser][Error] expected: 'try', but got:\n");
		list_print(copy);
		exit(1);
	}
	
	res->try_block = makeStmtBlock(copy, debug);
	
	if(res->try_block == NULL){
		list_print(copy);
		exit(1);
	}
	
	if(!list_expect(copy, CATCH)){
		printf("[Parser][Error] expected: 'catch', but got:\n");
		list_print(copy);
		exit(1);
	}
	
	res->catch_block = makeStmtBlock(copy, debug);
	
	if(res->catch_block == NULL){ 
		list_print(copy);
		exit(1);
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
