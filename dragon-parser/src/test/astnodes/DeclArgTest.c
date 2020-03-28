#include "DeclArgTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/DeclArg.h"

#include <stdbool.h>

int declarg_test_parse_declared_argument() {
	char* source = "((PInt)~>PInt) subr";

	struct TokenList* list = makeTokenList();

	list_add(list, LPARENS);
	list_add(list, LPARENS);
	list_add(list, makeToken(TYPEIDENTIFIER,"PInt"));
	list_add(list, LPARENS);
	list_add(list, ARROW);
	list_add(list, makeToken(TYPEIDENTIFIER,"PInt"));
	list_add(list, RPARENS);
	list_add(list, makeToken(ID,"subr"));

	struct DeclArg* node = makeDeclArg(list,false);

	bool assert1 = string("subr").compare( node->name) == 0;
	bool assert2 = (0 == list->size());

	return (assert1&&assert2)?1:0;
}

