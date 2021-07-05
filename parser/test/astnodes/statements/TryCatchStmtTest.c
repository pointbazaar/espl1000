#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "TryCatchStmtTest.h"

#include "statements/TryCatchStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int trycatch_stmt_test(bool debug){
	
	if(debug){
		printf("TEST: trycatch_stmt_test\n");
	}

	struct TokenList* tokens = makeTokenList();
	
	list_add(tokens, makeToken2(TRY,"try"));
	list_add(tokens, makeToken2(LCURLY,"{"));
	
	
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken2(ASSIGNOP,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken2(SEMICOLON, ";"));
	
	list_add(tokens, makeToken2(RCURLY,"}"));
	list_add(tokens, makeToken2(CATCH,"catch"));
	list_add(tokens, makeToken2(LCURLY,"{"));
	
	//
	
	list_add(tokens, makeToken2(RCURLY,"}"));

	struct TryCatchStmt* node = makeTryCatchStmt(tokens,debug);

	//all tokens should have been consumed
	assert(0 == list_size(tokens)); 
	assert(node != NULL);
	
	assert(node->try_block   != NULL);
	assert(node->catch_block != NULL);
	
	assert(node->try_block->count   == 1);
	assert(node->catch_block->count == 0);
	
	freeTokenList(tokens);
	freeTryCatchStmt(node);

	return 1;
}
