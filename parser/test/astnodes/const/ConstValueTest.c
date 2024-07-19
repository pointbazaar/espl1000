#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ConstValueTest.h"
#include "const/ConstValue.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_charconst_parse_char_constant_node() {

	status_test("test_charconst_parse_char_constant_node");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST, "'h'"));

	struct ConstValue* cv = makeConstValue(list);

	assert(cv != NULL);
	assert(cv->kind == 3);

	char node = cv->ptr.m3_char_const;

	assert('h' == node);

	freeTokenList(list);

	return 1;
}

int test_charconst_parse_char_constant_node_newline() {

	status_test("test_charconst_parse_char_constant_node_newline");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST, "'\n'"));

	struct ConstValue* cv = makeConstValue(list);

	assert(cv != NULL);
	assert(cv->kind == 3);

	char node = cv->ptr.m3_char_const;
	assert('\n' == node);

	freeTokenList(list);

	return 1;
}

int test_boolconst_parse_bool_constant_node() {

	status_test("test_boolconst_parse_bool_constant_node");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(BCONST_TRUE, "true"));

	struct ConstValue* cv = makeConstValue(list);

	assert(cv != NULL);
	assert(cv->kind == 1);

	bool b = cv->ptr.m1_bool_const;

	assert(b);

	freeTokenList(list);

	return 1;
}
