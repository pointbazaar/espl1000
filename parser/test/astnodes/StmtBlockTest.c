#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "StmtBlockTest.h"

#include "statements/Stmt.h"
#include "StmtBlock.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"


int test_stmtblock_1(bool debug){
	
	if(debug){
		printf("TEST: test_stmtblock_1\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(LCURLY,"{"));
	
	list_add(tokens, makeToken2(ID,"a") );
	list_add(tokens, makeToken2(ASSIGNOP,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken(SEMICOLON) );
	
	list_add(tokens, makeToken2(RCURLY,"}"));

	struct StmtBlock* node = makeStmtBlock(tokens,debug);

	assert(0 == list_size(tokens)); 
	//all tokens should have been consumed
	assert(node != NULL);
	
	freeTokenList(tokens);
	freeStmtBlock(node);

	return 1;
}
