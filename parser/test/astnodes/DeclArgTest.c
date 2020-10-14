#include "DeclArgTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/DeclArg.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int declarg_test_parse_declared_argument(bool debug) {

	if(debug){
		printf("TEST: declarg_test_parse_declared_argument\n");
	}
	
	//  ((PInt)~>PInt) subr

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(TYPEIDENTIFIER,"PInt"));
		list_add(list, makeToken2(RPARENS,")"));
		list_add(list, makeToken2(ARROW,"->"));
		list_add(list, makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add(list, makeToken2(RPARENS,")"));

	list_add(list, makeToken2(ID,"subr"));

	struct DeclArg* node = makeDeclArg(list,debug);

	assert(strcmp("subr", node->name) == 0);
	assert(0 == list_size(list));
	
	freeTokenList(list);
	freeDeclArg(node);

	return 1;
}

