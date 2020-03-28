#include "MethodTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Method.h"

#include <stdbool.h>

int method_test_can_parse_method_with_arguments() {

	struct TokenList* l = makeTokenList(); //ParserPhases.makeTokenList("fn main (String hello)~>PInt { main(); }", false);

	list_add( makeToken(FN));
	list_add( makeToken2(ID,"main"));
	list_add( makeToken(LPARENS));
	list_add( makeToken2(TYPEIDENTIFIER,"String"));
	list_add( makeToken2(ID,"hello"));
	list_add( makeToken(RPARENS));
	list_add( makeToken(ARROW));
	list_add( makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add( makeToken(LCURLY));
		list_add( makeToken2(ID,"main"));
		list_add( makeToken(LPARENS));
		list_add( makeToken(RPARENS));
		list_add( makeToken(SEMICOLON));

	list_add( makeToken(RCURLY));

	struct Method* m = makeMethod(&l, false);

	return  m->count_arguments;
}

int method_test_can_parse_subroutine() {

		struct TokenList* l = makeTokenList();

		list_add( makeToken(FN));
		list_add( makeToken2(ID,"main"));
		list_add( makeToken(LPARENS));
		list_add( makeToken(RPARENS));
		list_add( makeToken(ARROW));
		list_add( makeToken2(TYPEIDENTIFIER,"PInt"));
		list_add( makeToken(LCURLY));
			list_add( makeToken2(ID,"println"));
			list_add( makeToken(LPARENS));
			list_add( makeToken(RPARENS));
			list_add( makeToken(SEMICOLON));

			list_add( makeToken(RETURN));
			list_add( makeToken2(INTEGER,"0"));
			
		list_add( makeToken(RCURLY));

		struct Method* m = makeMethod(&l, false);
		return (m!=NULL)?1:0;
}

int method_test_can_parse_method_without_arguments() {

	struct TokenList l = makeTokenList();

	list_add( makeToken(FN));
	list_add( makeToken2(ID,"main"));
	list_add( makeToken(LPARENS));
	list_add( makeToken2(TYPEIDENTIFIER,"String"));
	list_add( makeToken2(ID,"hello"));
	list_add( makeToken(RPARENS));
	list_add( makeToken(ARROW));
	list_add( makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add( makeToken(LCURLY));
		list_add( makeToken2(ID,"main"));
		list_add( makeToken(LPARENS));
		list_add( makeToken(RPARENS));
		list_add( makeToken(SEMICOLON));

	list_add( makeToken(RCURLY));

	struct Method* m = makeMethod(&l, false);
	bool assert1 = (0 == m->count_arguments);

	return (assert1)?1:0;
}
