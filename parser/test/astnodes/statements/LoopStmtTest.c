#include <stdio.h>
#include <assert.h>

#include "LoopStmtTest.h"

#include "statements/LoopStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int loop_test1(bool debug){
	
	if(debug){
		printf("TEST: loop_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LOOP,"LOOP"));
	list_add(list, makeToken2(INTEGER,"3"));
	list_add(list, makeToken2(LCURLY,"{"));
	
		list_add(list, makeToken2(RETURN,"return"));
		list_add(list, makeToken2(INTEGER,"3"));
		list_add(list, makeToken2(SEMICOLON,";"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct LoopStmt* node = makeLoopStmt(list,debug);
	
	assert(node != NULL);
	
	freeTokenList(list);
	freeLoopStmt(node);

	return 1;
}
