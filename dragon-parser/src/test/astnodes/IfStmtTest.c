#include "IfStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/IfStmt.h"
#include "../../main/parsing/Expr.h"

#include <stdio.h>
#include <stdbool.h>

int if_test1(bool debug) {

	if(debug){
		printf("if_test1\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(IF,"if"));

	list_add(list, makeToken2(INTEGER, "5"));
	list_add(list, makeToken2(OPKEY, "<"));
	list_add(list, makeToken2(INTEGER, "3"));

	list_add(list, makeToken2(LCURLY,"{"));

	list_add(list, makeToken2(RCURLY,"}"));

	struct IfStmt* i = makeIfStmt(list,debug);

	bool a1 = (0 == list_size(list));
	if(i == NULL){return 0;}

	bool a2 = i->count_statements == 0;
	bool a3 = i->count_elseStatements == 0;

	struct Expr* condition = i->condition;
	if(condition == NULL){return 0;}

	if(condition->term1 == NULL){return 0;}
	if(condition->op == NULL){return 0;}
	if(condition->term2 == NULL){return 0;}
	
	return  (a1 && a2 && a3)?1:0;
}

int if_test2(bool debug) {

	if(debug){
		printf("if_test2\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(IF,"if"));

	list_add(l, makeToken2(ID,"x"));
	list_add(l, makeToken2(OPKEY,"<"));
	list_add(l, makeToken2(INTEGER,"5"));

	list_add(l, makeToken2(LCURLY,"{"));

	list_add(l, makeToken2(ID,"println"));


	list_add(l, makeToken2(LPARENS,"("));
	list_add(l, makeToken2(STRINGCONST,"x<5"));
	list_add(l, makeToken2(RPARENS,")"));

	list_add(l, makeToken2(SEMICOLON,";"));

	list_add(l, makeToken2(RCURLY,"}"));

	struct IfStmt* i = makeIfStmt(l,debug);

	if(i==NULL){return 0;}

	struct Expr* condition = i->condition;
	if(condition == NULL){return 0;}

	bool a1 = i->count_statements == 1;
	bool a2 = i->count_elseStatements == 0;

	return (a1 && a2)?1:0;
}
