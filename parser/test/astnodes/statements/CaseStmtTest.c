#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "CaseStmtTest.h"

#include "statements/CaseStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_parser_case_stmt() {

	status_test("test_parser_case_stmt");

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(CASE,"case"));
	
	list_add(list, makeToken2(BCONST_TRUE,"true"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct CaseStmt* node = makeCaseStmt(list);
	
	assert(node != NULL);
	assert(node->const_value->kind == 1);
	
	assert(node->const_value->ptr.m1_bool_const == true);
	assert(node->block != NULL);
	
	freeTokenList(list);
	free_case_stmt(node);

	return 1;
}
