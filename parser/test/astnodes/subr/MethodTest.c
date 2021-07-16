#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "MethodTest.h"

#include "subr/Method.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int method_test_can_parse_method_with_arguments(bool debug) {

	if(debug){
		printf("TEST: method_test_can_parse_method_with_arguments\n");
	}

	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(FN));
	list_add(l, makeToken2(ID,"main"));
	
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken2(TYPEID,"String"));
	list_add(l, makeToken2(ID,"hello"));
	list_add(l, makeToken(RPARENS));
	
	list_add(l, makeToken2(ARROW, "->"));
	list_add(l, makeToken2(TYPEID,"Car"));
	
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Method* m = makeMethod(l);
	if(
		m == NULL
		|| m->decl->count_args != 1
		|| m->block->count != 0
		|| m->block == NULL
		|| m->decl->args == NULL
		|| m->decl->args[0] == NULL
	){ 
		printf("method not complete \n");
		return 0;
	}
	
	assert(!m->decl->has_side_effects);
	assert(m->decl->count_args == 1);
	
	freeTokenList(l);
	free_method(m);

	return 1;
}

int method_test_can_parse_subroutine(bool debug) {

	if(debug){
		printf("TEST: method_test_can_parse_subroutine\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(FN,"fn"));
	list_add(l, makeToken2(ID,"main"));

	list_add(l, makeToken2(LPARENS,"("));
	list_add(l, makeToken2(RPARENS,")"));

	list_add(l, makeToken2(ARROW,"~>"));
	list_add(l, makeToken2(TYPEID,"Apple"));

	list_add(l, makeToken2(LCURLY,"{"));

		list_add(l, makeToken2(ID,"println"));
		list_add(l, makeToken2(LPARENS,"("));
		list_add(l, makeToken2(RPARENS,")"));
		list_add(l, makeToken2(SEMICOLON,";"));

		list_add(l, makeToken2(RETURN,"return"));
		list_add(l, makeToken2(INTEGER,"0"));
		list_add(l, makeToken2(SEMICOLON,";"));
		
	list_add(l, makeToken2(RCURLY,"}"));

	struct Method* m = makeMethod(l);
	assert(m != NULL);
	assert(m->decl->count_args == 0);
	assert(m->decl->has_side_effects);
	
	freeTokenList(l);
	free_method(m);

	return 1;
}

int method_test_can_parse_method_without_arguments(bool debug) {

	if (debug) {
		printf("TEST: method_test_can_parse_method_without_arguments\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(FN,"fn"));
	list_add(l, makeToken2(ID,"main"));

	list_add(l, makeToken2(LPARENS,"("));
	list_add(l, makeToken2(RPARENS,")"));

	list_add(l, makeToken2(ARROW,"->"));

	list_add(l, makeToken2(TYPEID,"MyType"));

	list_add(l, makeToken2(LCURLY,"{"));

	list_add(l, makeToken2(RCURLY,"}"));

	struct Method* m = makeMethod(l);
	assert(m != NULL);
	assert(0 == m->decl->count_args);
	assert(!m->decl->has_side_effects);
	
	freeTokenList(l);
	free_method(m);

	return 1;
}
