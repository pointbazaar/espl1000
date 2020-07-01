#include "RetStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/RetStmt.h"

#include <stdio.h>
#include <stdbool.h>

int retstmt_test1(bool debug) {

	printf("retstmt_test1\n");

		struct TokenList* list = makeTokenList();

		list_add(list, makeToken(RETURN));

		list_add(list, makeToken(LPARENS));

		list_add(list, makeToken2(OPKEY,"-"));
		list_add(list, makeToken2(INTEGER,"5"));

		list_add(list, makeToken(RPARENS));

		list_add(list, makeToken(SEMICOLON));

		struct RetStmt* r = makeRetStmt(list,debug);

		return (r!=NULL)?1:0;
}

int retstmt_test2(bool debug){

	printf("retstmt_test2\n");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(RETURN));

	list_add(list, makeToken(LPARENS));

	list_add(list, makeToken2(OPKEY,"-"));

	list_add(list, makeToken2(INTEGER,"5"));

	list_add(list, makeToken(RPARENS));

	list_add(list, makeToken2(OPKEY,"*"));

	list_add(list, makeToken2(ID,"n"));

	list_add(list, makeToken(SEMICOLON));

	struct RetStmt* r = makeRetStmt(list,debug);

	return (r!=NULL)?1:0;
}

int retstmt_test3(bool debug) {

	printf("retstmt_test3\n");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(RETURN));

	list_add(list, makeToken2(ID,"arr"));

	list_add(list, makeToken(LBRACKET));
	list_add(list, makeToken2(INTEGER,"0"));

	list_add(list, makeToken(RBRACKET));

	list_add(list, makeToken(SEMICOLON));

	struct RetStmt* r = makeRetStmt(list,debug);

	return (r!=NULL)?1:0;
}
