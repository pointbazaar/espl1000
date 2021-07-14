#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "BoolConstTest.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "const/BoolConst.h"

#include "ast/util/free_ast.h"

int boolconst_test_parse_bool_constant_node(bool debug)  {

	if(debug){
		printf("TEST: boolconst_test_parse_bool_constant_node\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(BCONST_TRUE,"true"));

	struct BoolConst* b = makeBoolConst(list);

	if(b == NULL){ return 0;}

	assert(b->value);

	free_bool_const(b);
	freeTokenList(list);

	return 1;
}

