#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "SubrTypeTest.h"

#include "types/SubrType.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int subrtype_test_typename() {

	status_test("subrtype_test_typename");

	//(uint,uint)->uint
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"Carrot") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEID,"Carrot") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken2(ARROW,"->") );
	list_add(l, makeToken2(TYPEID,"MyStruct") );

	struct SubrType* sub = makeSubrType(l);

	assert(0 == list_size(l));
	assert(sub != NULL);
	
	freeTokenList(l);
	free_subr_type(sub);

	return 1;
}

int subrtype_test_typename_subroutine_return_type() {

	status_test("subrtype_test_typename_subroutine_return_type");

	//(uint,uint)->((uint)->uint) 
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken2(TYPEID,"uint") );
		list_add(l, makeToken(COMMA) );
		list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );

	list_add(l, makeToken2(ARROW,"->") );

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken(LPARENS) );
			list_add(l, makeToken2(TYPEID,"uint") );
		list_add(l, makeToken(RPARENS) );

		list_add(l, makeToken2(ARROW,"->") );
		list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );

	struct SubrType* sub = makeSubrType(l);

	assert(0 == list_size(l));
	assert(sub != NULL);
	
	freeTokenList(l);
	free_subr_type(sub);

	return 1;
}

int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects() {

	status_test("subrtype_test_subroutine_type_parsing_subroutine_with_side_effects");

	//(uint,MyType)~>uint
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEID,"MyType") );
	list_add(l, makeToken(RPARENS) );
	
	list_add(l, makeToken2(ARROW,"->"));
	
	list_add(l, makeToken2(TYPEID,"uint") );

	struct SubrType* node = makeSubrType(l);
	assert(node != NULL);

	assert(0 == list_size(l));
	
	freeTokenList(l);
	free_subr_type(node);

	return 1;
}

int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects() {

	status_test("subrtype_test_subroutine_type_parsing_subroutine_without_side_effects");

	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEID,"uint") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken2(ARROW,"->") );
	list_add(l, makeToken2(TYPEID,"uint") );

	struct SubrType* node = makeSubrType(l);

	assert(0 == list_size(l));
	assert(node != NULL);
	
	freeTokenList(l);
	free_subr_type(node);

	return 1;
}

