#include "SubrTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/typenodes/SubrType.h"

#include <stdbool.h>

int subrtype_test_typename() {
	//(PInt,PInt)->PInt
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* sub = makeSubrType(l,false);

	bool assert1 = (0 == list_size(l));

	return (assert1)?1:0;
}

int subrtype_test_typename_subroutine_return_type() {
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

	struct SubrType* sub = makeSubrType(l,false);

	bool assert1 = (0 == list_size(l));
	return (assert1)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects()  {
	//(PInt,MyType)~>PInt
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(COMMA) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"MyType") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* node = makeSubrType(l,false);

	bool assert1 = (0 == list_size(l));
	return (assert1)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects() {
	//(PInt)->PInt
	struct TokenList* l = makeTokenList(); 

	list_add(l, makeToken(LPARENS) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(l, makeToken(RPARENS) );
	list_add(l, makeToken(ARROW) );
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt") );

	struct SubrType* node = makeSubrType(l,false);

	bool assert1 = (0 == list_size(l));
	return (assert1)?1:0;
}

