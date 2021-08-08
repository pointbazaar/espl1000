#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "BoolConstTest.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "const/BoolConst.h"

#include "ast/util/free_ast.h"

int test_boolconst_parse_bool_constant_node() {

	status_test("test_boolconst_parse_bool_constant_node");

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(BCONST_TRUE,"true"));

	struct BoolConst* b = makeBoolConst(list);

	if(b == NULL){ return 0;}

	assert(b->value);

	free_bool_const(b);
	freeTokenList(list);

	return 1;
}

