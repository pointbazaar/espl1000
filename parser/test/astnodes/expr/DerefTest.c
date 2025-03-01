#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ParseExprTests.h"

#include "expr/Deref.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_deref() {

	status_test((char*)__func__);

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(OPKEY_ARITHMETIC_MUL));
	list_add(list, makeToken2(ID, "x"));

	struct Deref* d = makeDeref2(list);
	assert(d != NULL);

	freeTokenList(list);
	free_deref(d);

	return 1;
}
