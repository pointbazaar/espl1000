#include "TermTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"

#include "expr/Term.h"
#include "var/Variable.h"
#include "var/SimpleVar.h"

#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

int term_test_simple_term(bool debug) {

	if(debug){
		printf("TEST: term_test_simple_term()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	
	struct Term* t = makeTerm(list,debug);
	assert(t != NULL);
	
	freeTokenList(list);
	freeTerm(t);
	
	return 1;
}

int term_test_variable_term(bool debug) {

	if(debug){
		printf("TEST: term_test_variable_term()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x"));

	struct Term* t = makeTerm(list,debug);
	assert(t != NULL);

	assert(list_size(list) == 0);

	struct Variable* v = t->ptr.m6;
	assert(v != NULL);

	struct SimpleVar* sv = v->simpleVar;
	assert(sv != NULL);

	assert(strcmp(sv->name,"x") == 0);
	
	freeTokenList(list);
	freeTerm(t);

	return 1;
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
	assert(t != NULL);
	
	struct Expr* expr = t->ptr.m5;
	assert(expr != NULL);

	assert(list_size(list) == 0);

	struct Variable* v = expr->term1->term->ptr.m6;
	assert(v != NULL);

	struct SimpleVar* sv = v->simpleVar;
	assert(sv != NULL);

	assert(strcmp(sv->name,"x") == 0);
	
	freeTokenList(list);
	freeTerm(t);

	return 1;
}
