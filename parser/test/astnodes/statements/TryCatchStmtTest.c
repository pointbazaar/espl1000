#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "TryCatchStmtTest.h"

#include "statements/TryCatchStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int trycatch_stmt_test() {

	status_test("trycatch_stmt_test");

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

	struct TryCatchStmt* node = makeTryCatchStmt(tokens);

	//all tokens should have been consumed
	assert(0 == list_size(tokens)); 
	assert(node != NULL);
	
	assert(node->try_block   != NULL);
	assert(node->catch_block != NULL);
	
	assert(node->try_block->count   == 1);
	assert(node->catch_block->count == 0);
	
	freeTokenList(tokens);
	free_try_catch_stmt(node);

	return 1;
}
