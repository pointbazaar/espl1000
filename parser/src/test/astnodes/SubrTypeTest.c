#include "SubrTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/typenodes/SubrType.h"
#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>

int subrtype_test_typename(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_typename\n");
	}

	//(PInt,PInt)->PInt
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* sub = makeSubrType(l,debug);

	bool assert1 = (0 == list_size(l));
	bool assert2 = sub != NULL;
	
	freeTokenList(l);
	freeSubrType(sub);

	return (assert1 && assert2)?1:0;
}

int subrtype_test_typename_subroutine_return_type(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_typename_subroutine_return_type\n");
	}

	//(PInt,PInt)->((PInt)->PInt) 
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
		list_add(l, makeToken(COMMA) );
		list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );

	list_add(l, makeToken(ARROW) );

	list_add(l, makeToken(LPARENS) );
		list_add(l, makeToken(LPARENS) );
			list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
		list_add(l, makeToken(RPARENS) );

		list_add(l, makeToken(ARROW) );
		list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );

	struct SubrType* sub = makeSubrType(l,debug);

	bool assert1 = (0 == list_size(l));
	bool assert2 = sub != NULL;
	
	freeTokenList(l);
	freeSubrType(sub);

	return (assert1 && assert2)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects(bool debug)  {

	if(debug){
		printf("TEST: subrtype_test_subroutine_type_parsing_subroutine_with_side_effects\n");
	}

	//(PInt,MyType)~>PInt
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"MyType") );
	list_add(l, makeToken(RPARENS) );
	
	list_add(l, makeToken(ARROW) );
	
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* node = makeSubrType(l,debug);
	if(node == NULL){ return 0; }

	bool assert1 = (0 == list_size(l));
	
	freeTokenList(l);
	freeSubrType(node);

	return (assert1)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects(bool debug) {

	if(debug){
		printf("TEST: subrtype_test_subroutine_type_parsing_subroutine_without_side_effects\n");
	}

	//(PInt)->PInt
	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* node = makeSubrType(l,debug);

	bool assert1 = (0 == list_size(l));
	bool assert2 = node != NULL;
	
	freeTokenList(l);
	freeSubrType(node);

	return (assert1 && assert2)?1:0;
}

