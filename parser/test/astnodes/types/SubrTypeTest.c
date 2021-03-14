#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "SubrTypeTest.h"

#include "types/SubrType.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int subrtype_test_typename(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_typename\n");
	}

	//(uint,uint)->uint
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"Carrot") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEID,"Carrot") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEID,"MyStruct") );

	struct SubrType* sub = makeSubrType(l,debug);

	assert(0 == list_size(l));
	assert(sub != NULL);
	
	freeTokenList(l);
	freeSubrType(sub);

	return 1;
}

int subrtype_test_typename_subroutine_return_type(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_typename_subroutine_return_type\n");
	}

	//(uint,uint)->((uint)->uint) 
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken2(TYPEID,"uint") );
		list_add(l, makeToken(COMMA) );
		list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );

	list_add(l, makeToken(ARROW) );

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken(LPARENS) );
			list_add(l, makeToken2(TYPEID,"uint") );
		list_add(l, makeToken(RPARENS) );

		list_add(l, makeToken(ARROW) );
		list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );

	struct SubrType* sub = makeSubrType(l,debug);

	assert(0 == list_size(l));
	assert(sub != NULL);
	
	freeTokenList(l);
	freeSubrType(sub);

	return 1;
}

int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects(bool debug)  {

	if(debug){
		printf("TEST: subrtype_test_subroutine_type_parsing_subroutine_with_side_effects\n");
	}

	//(uint,MyType)~>uint
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEID,"MyType") );
	list_add(l, makeToken(RPARENS) );
	
	list_add(l, makeToken(ARROW) );
	
	list_add(l, makeToken2(TYPEID,"uint") );

	struct SubrType* node = makeSubrType(l,debug);
	assert(node != NULL);

	assert(0 == list_size(l));
	
	freeTokenList(l);
	freeSubrType(node);

	return 1;
}

int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_subroutine_type_parsing_subroutine_without_side_effects\n");
	}

	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEID,"uint") );

	struct SubrType* node = makeSubrType(l,debug);

	assert(0 == list_size(l));
	assert(node != NULL);
	
	freeTokenList(l);
	freeSubrType(node);

	return 1;
}

