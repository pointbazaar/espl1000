#include "DeclArgTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/DeclArg.h"

#include <stdbool.h>

int declarg_test_parse_declared_argument(bool debug) {
	
	//  ((PInt)~>PInt) subr

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken(ARROW));
	list_add(list, makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add(list, makeToken(RPARENS));
	list_add(list, makeToken2(ID,"subr"));

	struct DeclArg* node = makeDeclArg(list,false);

	bool assert1 = strcmp("subr", node->name) == 0;
	bool assert2 = (0 == list_size(list));

	return (assert1&&assert2)?1:0;
}

