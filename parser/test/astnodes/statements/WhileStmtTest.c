#include "WhileStmtTest.h"
#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/parsing/statements/WhileStmt.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int whilestmt_test1(bool debug) {

	if(debug){
		printf("TEST: whilestmt_test1\n");
	}
	
	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(WHILE) );

	list_add(list, makeToken(LPARENS) );

	list_add(list, makeToken2(INTEGER,"5") );
	list_add(list, makeToken2(OPKEY,"<") );
	list_add(list, makeToken2(INTEGER,"3") );

	list_add(list, makeToken(RPARENS) );

	list_add(list, makeToken(LCURLY) );

	list_add(list, makeToken(RCURLY) );

	struct WhileStmt* ws = makeWhileStmt(list,debug);
	assert(ws != NULL);
	
	assert(ws->block->count == 0);
	assert(ws->condition != NULL);
	
	freeTokenList(list);
	freeWhileStmt(ws);
	
	return 1;
}

int whilestmt_test2(bool debug){
	
	if(debug){
		printf("TEST: whilestmt_test2\n");
	}
	
	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(WHILE) );

	list_add(list, makeToken(LPARENS) );

	list_add(list, makeToken2(INTEGER,"x") );
	list_add(list, makeToken2(OPKEY,"<") );
	list_add(list, makeToken2(INTEGER,"4") );

	list_add(list, makeToken(RPARENS) );

	list_add(list, makeToken(LCURLY) );

	list_add(list, makeToken(RCURLY) );

	struct WhileStmt* ws = makeWhileStmt(list,debug);
	assert(ws != NULL);
	
	assert(ws->block->count == 0);
	assert(ws->condition != NULL);
	
	struct Expr* e = ws->condition;
	assert(e->term1 != NULL);
	
	freeTokenList(list);
	freeWhileStmt(ws);

	return 1;
}
