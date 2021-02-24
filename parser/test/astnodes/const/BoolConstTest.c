#include "BoolConstTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../../main/astnodes/const/BoolConst.h"
#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int boolconst_test_parse_bool_constant_node(bool debug)  {

	if(debug){
		printf("TEST: boolconst_test_parse_bool_constant_node\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(BCONST_TRUE,"true"));

	struct BoolConst* b = makeBoolConst(list,debug);

	if(b == NULL){ return 0;}

	assert(b->value);
	
	freeBoolConst(b);
	freeTokenList(list);

	return 1;
}

