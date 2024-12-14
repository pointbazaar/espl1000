#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "WhileStmtTest.h"

#include "statements/WhileStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int whilestmt_test1() {

	status_test("whilestmt_test1");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(WHILE));

	list_add(list, makeToken(LPARENS));

	list_add(list, makeToken2(INTEGER, "5"));
	list_add(list, makeToken(OPKEY_RELATIONAL_LT));
	list_add(list, makeToken2(INTEGER, "3"));

	list_add(list, makeToken(RPARENS));

	list_add(list, makeToken(LCURLY));

	list_add(list, makeToken(RCURLY));

	struct WhileStmt* ws = makeWhileStmt(list);
	assert(ws != NULL);

	assert(ws->block->count == 0);
	assert(ws->condition != NULL);

	freeTokenList(list);
	free_while_stmt(ws);

	return 1;
}

int whilestmt_test2() {

	status_test("whilestmt_test2");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(WHILE));

	list_add(list, makeToken(LPARENS));

	list_add(list, makeToken2(INTEGER, "x"));
	list_add(list, makeToken(OPKEY_RELATIONAL_LT));
	list_add(list, makeToken2(INTEGER, "4"));

	list_add(list, makeToken(RPARENS));

	list_add(list, makeToken(LCURLY));

	list_add(list, makeToken(RCURLY));

	struct WhileStmt* ws = makeWhileStmt(list);
	assert(ws != NULL);

	assert(ws->block->count == 0);
	assert(ws->condition != NULL);

	struct Expr* e = ws->condition;
	assert(e->term1 != NULL);

	freeTokenList(list);
	free_while_stmt(ws);

	return 1;
}
