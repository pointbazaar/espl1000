#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "LambdaTest.h"

#include "Lambda.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

void test_lambda(){

	status_test("test_lambda");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LPARENS,"("));
	
	list_add(list, makeToken2(ID,"a"));
	list_add(list, makeToken2(COMMA, ","));
	list_add(list, makeToken2(ID,"b"));
	
	list_add(list, makeToken2(RPARENS,")"));
	
	list_add(list, makeToken2(ARROW,"->"));

	list_add(list, makeToken2(INTEGER,"0"));

	struct Lambda* r = makeLambda(list);
	
	assert(r != NULL);
	assert(r->count_params == 2);
	assert(r->params[0] != NULL);
	assert(r->params[1] != NULL);

	assert(r->expr != NULL);

	freeTokenList(list);
	free_lambda(r);
}
