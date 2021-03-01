#include <stdio.h>
#include <assert.h>

#include "BreakStmtTest.h"

#include "statements/BreakStmt.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"


int break_test1(bool debug){
	
	if(debug){
		printf("TEST: break_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(BREAK,"BREAK"));
	list_add(list, makeToken2(SEMICOLON,";"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct BreakStmt* node = makeBreakStmt(list,debug);
	
	assert(node != NULL);
	
	freeTokenList(list);
	freeBreakStmt(node);

	return 1;
}
