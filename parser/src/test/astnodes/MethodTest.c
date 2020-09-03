#include "MethodTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../main/parsing/Method.h"

#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>

int method_test_can_parse_method_with_arguments(bool debug) {

	if(debug){
		printf("TEST: method_test_can_parse_method_with_arguments\n");
	}

	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(FN));
	list_add(l, makeToken2(ID,"main"));
	
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken2(TYPEIDENTIFIER,"String"));
	list_add(l, makeToken2(ID,"hello"));
	list_add(l, makeToken(RPARENS));
	
	list_add(l, makeToken(ARROW));
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt"));
	
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Method* m = makeMethod(l, debug);
	if(
		m == NULL
		|| m->count_args != 1
		|| m->block->count != 0
		|| m->block == NULL
		|| m->args == NULL
		|| m->args[0] == NULL
	){ 
		printf("method not complete \n");
		return 0;
	}
	
	if(debug){
		printf("free stuff\n");
	}
	
	const bool a1 = m->count_args == 1;
	
	freeTokenList(l);
	freeMethod(m);

	return  a1;
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

	list_add(l, makeToken2(ARROW,"->"));
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt"));

	list_add(l, makeToken2(LCURLY,"{"));

		list_add(l, makeToken2(ID,"println"));
		list_add(l, makeToken2(LPARENS,"("));
		list_add(l, makeToken2(RPARENS,")"));
		list_add(l, makeToken2(SEMICOLON,";"));

		list_add(l, makeToken2(RETURN,"return"));
		list_add(l, makeToken2(INTEGER,"0"));
		list_add(l, makeToken2(SEMICOLON,";"));
		
	list_add(l, makeToken2(RCURLY,"}"));

	struct Method* m = makeMethod(l, debug);
	if(m == NULL){
		printf("Method could not be parsed\n");
		return 0;
	}

	bool a1 = m->count_args == 0;

	if(!a1){
		printf("Assertion failed: m did not have 0 arguments\n");
	}
	
	freeTokenList(l);
	freeMethod(m);

	return (a1)?1:0;
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

	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt"));

	list_add(l, makeToken2(LCURLY,"{"));

	list_add(l, makeToken2(RCURLY,"}"));

	struct Method* m = makeMethod(l, debug);
	if(m == NULL){return 0;}

	bool assert1 = (0 == m->count_args);
	
	freeTokenList(l);
	freeMethod(m);

	return (assert1)?1:0;
}
