#include "WhileStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/statements/WhileStmt.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>

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
	if(ws == NULL){return 0;}
	
	
	bool a1 = ws->block->count == 0;
	bool a2 = ws->condition != NULL;
	
	freeTokenList(list);
	freeWhileStmt(ws);
	
	return (a1 && a2)?1:0;
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
	if(ws == NULL){ return 0; }
	
	bool a1 = ws->block->count == 0;
	bool a2 = ws->condition != NULL;
	
	struct Expr* e = ws->condition;
	
	if(e->term1 == NULL){
		return 0;
	}
	
	freeTokenList(list);
	freeWhileStmt(ws);

	return (a1 && a2)?1:0;
}
