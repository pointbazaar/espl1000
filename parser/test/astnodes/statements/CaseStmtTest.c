#include <stdio.h>
#include <assert.h>

#include "CaseStmtTest.h"

#include "statements/CaseStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_parser_case_stmt(bool debug) {

	if(debug){
		printf("TEST: test_parser_case_stmt\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(CASE,"case"));
	
	list_add(list, makeToken2(BCONST_TRUE,"true"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct CaseStmt* node = makeCaseStmt(list, debug);
	
	assert(node != NULL);
	assert(node->kind == 0);
	
	assert(node->ptr.m1 != NULL);
	assert(node->block != NULL);
	
	freeTokenList(list);
	freeCaseStmt(node);

	return 1;
}
