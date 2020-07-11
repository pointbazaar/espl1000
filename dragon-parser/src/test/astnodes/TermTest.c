#include "TermTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Term.h"
#include "../../main/parsing/Variable.h"
#include "../../main/parsing/SimpleVar.h"

#include <stdio.h>
#include <stdbool.h>

int term_test_simple_term(bool debug) {

	if(debug){
		printf("TEST: term_test_simple_term()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	struct Term* expr = makeTerm(list,debug);
	return (expr!=NULL)?1:0;
}

int term_test_variable_term(bool debug) {

	if(debug){
		printf("TEST: term_test_variable_term()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x"));

	struct Term* t = makeTerm(list,debug);
	if(t == NULL){return 0;}

	bool a1 = list_size(list) == 0;

	struct Variable* v = t->m6;
	if(v == NULL){return 0;}

	struct SimpleVar* sv = v->simpleVariableNode;
	if(sv == NULL){return 0;}

	bool a2 = strcmp(sv->name,"x") == 0;

	return (a1 && a2)?1:0;
}


int term_test_parentheses(bool debug){
	
	if(debug){
		printf("TEST: term_test_parentheses()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(LPARENS, "("));
	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(RPARENS, ")"));

	struct Term* t = makeTerm(list, debug);
	if(t == NULL){return 0;}
	
	struct Expr* expr = t->m5;
	if(expr == NULL){ return 0;}

	const bool a1 = list_size(list) == 0;

	struct Variable* v = expr->term1->m6;
	if(v == NULL){return 0;}

	struct SimpleVar* sv = v->simpleVariableNode;
	if(sv == NULL){return 0;}

	const bool a2 = strcmp(sv->name,"x") == 0;

	return (a1 && a2)?1:0;
}
