#include <assert.h>
#include <parser/test/test_parser_util.h>
#include <stdbool.h>
#include <stdio.h>

#include "Range.h"
#include "RangeTest.h"
#include "ast/util/free_ast.h"
#include "token/TokenKeys.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"

void range_test1() {

	status_test("range_test1");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID, "a"));

	list_add(list, makeToken2(RANGEOP, ".."));

	list_add(list, makeToken2(INTEGER, "0"));

	list_add(list, makeToken(SEMICOLON));

	struct Range* r = makeRange(list);

	assert(r != NULL);
	assert(r->start != NULL);
	assert(r->end != NULL);

	freeTokenList(list);
	free_range(r);
}
