#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "CharConstTest.h"
#include "const/CharConst.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_charconst_parse_char_constant_node() {

	status_test("test_charconst_parse_char_constant_node");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"'h'"));

	struct CharConst* node = makeCharConst(list);
	
	assert(node != NULL);
	assert('h'== node->value);

	
	freeTokenList(list);
	free_char_const(node);

	return 1;
}

int test_charconst_parse_char_constant_node_newline() {


	status_test("test_charconst_parse_char_constant_node_newline");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"'\n'"));

	struct CharConst* node = makeCharConst(list);
	assert('\n' == node->value);
	
	freeTokenList(list);
	free_char_const(node);

	return 1;
}
