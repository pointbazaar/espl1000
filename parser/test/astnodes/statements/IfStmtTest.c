#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "IfStmtTest.h"

#include "statements/IfStmt.h"
#include "expr/Expr.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int if_test1(bool debug) {

	if(debug){
		printf("TEST: if_test1\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(IF,"if"));

	list_add(list, makeToken2(INTEGER, "5"));
	list_add(list, makeToken2(OPKEY_RELATIONAL, "<"));
	list_add(list, makeToken2(INTEGER, "3"));

	list_add(list, makeToken2(LCURLY,"{"));

	list_add(list, makeToken2(RCURLY,"}"));

	struct IfStmt* i = makeIfStmt(list,debug);

	assert(0 == list_size(list));
	assert(i != NULL);
	assert(i->block->count == 0);
	assert(i->elseBlock == NULL);

	struct Expr* condition = i->condition;
	assert(condition != NULL);

	assert(condition->term1 != NULL);
	assert(condition->op != NULL);
	assert(condition->term2 != NULL);
	
	freeTokenList(list);
	freeIfStmt(i);
	
	return 1;
}

int if_test2(bool debug) {

	if(debug){
		printf("TEST: if_test2\n");
	}

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

	struct IfStmt* i = makeIfStmt(l,debug);

	assert(i != NULL);

	struct Expr* condition = i->condition;
	assert(condition != NULL);

	assert(i->block->count == 1);
	assert(i->elseBlock == NULL);
	
	freeTokenList(l);
	freeIfStmt(i);

	return 1;
}
