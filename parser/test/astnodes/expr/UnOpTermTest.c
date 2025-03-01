#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ParseExprTests.h"

#include "expr/UnOpTerm.h"
#include "var/Variable.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_unop_with() {

	status_test("test_unop_with");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(OPKEY_LOGICAL_NOT));
	list_add(list, makeToken2(INTEGER, "4"));

	struct UnOpTerm* t = makeUnOpTerm(list);
	assert(t != NULL);

	freeTokenList(list);
	free_un_op_term(t);

	return 1;
}

int test_unop_without() {

	status_test("test_unop_without");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER, "4"));

	struct UnOpTerm* t = makeUnOpTerm(list);
	assert(t != NULL);

	freeTokenList(list);
	free_un_op_term(t);

	return 1;
}
