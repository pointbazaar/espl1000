#include "IfStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/IfStmt.h"

#include <stdio.h>
#include <stdbool.h>

int if_test1(bool debug) {

	if(debug){
		printf("if_test1\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, IF);

	list_add(list, makeToken(INTEGER, "5"));
	list_add(list, makeToken(OPKEY, "<"));
	list_add(list, makeToken(INTEGER, "3"));

	list_add(list, LCURLY);

	list_add(list, RCURLY);

	struct IfStmt* i = makeIfStmt(list,debug);
	bool assert1 = (0 == list->size());

	return  (assert1)?1:0;
}

int if_test2(bool debug) {

	if(debug){
		printf("if_test2\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(IF));

	//l->add(LPARENS);

	list_add(l, makeToken(ID,"x"));
	list_add(l, makeToken2(OPKEY,"<"));
	list_add(l, makeToken2(INTEGER,"5"));

	//l->add(RPARENS);

	list_add(l, makeToken(LCURLY));

	list_add(l, makeToken2(ID,"println"));


	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken2(STRINGCONST,"x<5"));
	list_add(l, makeToken(RPARENS));

	list_add(l, makeToken(SEMICOLON));

	list_add(l, makeToken(RCURLY));

	struct IfStmt* i = makeIfStmt(l,debug);

	return (i!=NULL)?1:0;
}
