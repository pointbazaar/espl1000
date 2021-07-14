#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "RetStmtTest.h"

#include "statements/RetStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int retstmt_test1(bool debug) {

	if(debug){
		printf("TEST: retstmt_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(RETURN));

	list_add(list, makeToken(LPARENS));

	list_add(list, makeToken2(OPKEY_ARITHMETIC,"-"));
	list_add(list, makeToken2(INTEGER,"5"));

	list_add(list, makeToken(RPARENS));

	list_add(list, makeToken(SEMICOLON));

	struct RetStmt* r = makeRetStmt(list,debug);
	assert(r != NULL);
	
	freeTokenList(list);
	free_ret_stmt(r);

	return 1;
}

int retstmt_test2(bool debug){

	if(debug){
		printf("TEST: retstmt_test2\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(RETURN));

	list_add(list, makeToken(LPARENS));

	list_add(list, makeToken2(OPKEY_ARITHMETIC,"-"));

	list_add(list, makeToken2(INTEGER,"5"));

	list_add(list, makeToken(RPARENS));

	list_add(list, makeToken2(OPKEY_ARITHMETIC,"*"));

	list_add(list, makeToken2(ID,"n"));

	list_add(list, makeToken(SEMICOLON));

	struct RetStmt* r = makeRetStmt(list,debug);
	assert(r != NULL);
	
	freeTokenList(list);
	free_ret_stmt(r);

	return 1;
}

int retstmt_test3(bool debug) {

	if(debug){
		printf("TEST: retstmt_test3\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(RETURN));

	list_add(list, makeToken2(ID,"arr"));

	list_add(list, makeToken(LBRACKET));
	list_add(list, makeToken2(INTEGER,"0"));

	list_add(list, makeToken(RBRACKET));

	list_add(list, makeToken(SEMICOLON));

	struct RetStmt* r = makeRetStmt(list,debug);
	assert(r != NULL);
	
	freeTokenList(list);
	free_ret_stmt(r);

	return 1;
}
