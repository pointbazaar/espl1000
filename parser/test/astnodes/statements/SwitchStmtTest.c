#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "SwitchStmtTest.h"

#include "statements/SwitchStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_switch(){

	status_test("test_switch");

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(SWITCH,"switch"));
	list_add(list, makeToken2(ID,"a"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(CASE,"case"));
	list_add(list, makeToken2(INTEGER,"4"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	list_add(list, makeToken2(RCURLY,"}"));
	
	list_add(list, makeToken2(RCURLY,"}"));
	
	struct SwitchStmt* t = makeSwitchStmt(list);
	
	assert(t != NULL);
	assert(t->count_cases == 1);
	assert(t->expr != NULL);
	
	freeTokenList(list);
	free_switch_stmt(t);
	
	return 1;
}
