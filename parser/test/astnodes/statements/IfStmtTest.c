#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "IfStmtTest.h"

#include "statements/IfStmt.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int if_test1() {

	status_test("if_test1");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(IF,"if"));

	list_add(list, makeToken2(INTEGER, "5"));
	list_add(list, makeToken2(OPKEY_RELATIONAL, "<"));
	list_add(list, makeToken2(INTEGER, "3"));

	list_add(list, makeToken2(LCURLY,"{"));

	list_add(list, makeToken2(RCURLY,"}"));

	struct IfStmt* i = makeIfStmt(list);

	assert(0 == list_size(list));
	assert(i != NULL);
	assert(i->block->count == 0);
	assert(i->else_block == NULL);

	struct Expr* condition = i->condition;
	assert(condition != NULL);

	assert(condition->term1 != NULL);
	assert(condition->op != NULL);
	assert(condition->term2 != NULL);
	
	freeTokenList(list);
	free_if_stmt(i);
	
	return 1;
}

int if_test2() {

	status_test("if_test2");

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(IF,"if"));

	list_add(l, makeToken2(ID,"x"));
	list_add(l, makeToken2(OPKEY_RELATIONAL,"<"));
	list_add(l, makeToken2(INTEGER,"5"));

	list_add(l, makeToken2(LCURLY,"{"));

	list_add(l, makeToken2(ID,"println"));


	list_add(l, makeToken2(LPARENS,"("));
	list_add(l, makeToken2(INTEGER,"5"));
	list_add(l, makeToken2(RPARENS,")"));

	list_add(l, makeToken2(SEMICOLON,";"));

	list_add(l, makeToken2(RCURLY,"}"));

	struct IfStmt* i = makeIfStmt(l);

	assert(i != NULL);

	struct Expr* condition = i->condition;
	assert(condition != NULL);

	assert(i->block->count == 1);
	assert(i->else_block == NULL);
	
	freeTokenList(l);
	free_if_stmt(i);

	return 1;
}
