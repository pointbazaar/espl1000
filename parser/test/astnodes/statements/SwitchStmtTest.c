#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "SwitchStmtTest.h"

#include "statements/SwitchStmt.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int test_switch(bool debug){

	if(debug){
		printf("TEST: test_switch()\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(SWITCH,"switch"));
	list_add(list, makeToken2(ID,"a"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(CASE,"case"));
	list_add(list, makeToken2(INTEGER,"4"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	list_add(list, makeToken2(RCURLY,"}"));
	
	list_add(list, makeToken2(RCURLY,"}"));
	
	struct SwitchStmt* t = makeSwitchStmt(list,debug);
	
	assert(t != NULL);
	assert(t->count_cases == 1);
	assert(t->var != NULL);
	
	freeTokenList(list);
	freeSwitchStmt(t);
	
	return 1;
}
