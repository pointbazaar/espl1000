#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "FloatConstTest.h"
#include "const/FloatConst.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_floatconst_1() {

	status_test("test_floatconst_1");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list);
	assert(node != NULL);
	
	freeTokenList(list);
	free_float_const(node);

	return 1;
}

int test_floatconst_2() {

	status_test("test_floatconst_2");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY_ARITHMETIC,"-"));
	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list);
	
	assert(node != NULL);
	
	freeTokenList(list);
	free_float_const(node);
	
	return 1;
}
