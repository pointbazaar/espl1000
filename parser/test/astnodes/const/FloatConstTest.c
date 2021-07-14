#include <stdio.h>
#include <assert.h>

#include "FloatConstTest.h"
#include "const/FloatConst.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int floatconst_test1(bool debug) {

	if(debug){
		printf("TEST: floatconst_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list);
	assert(node != NULL);
	
	freeTokenList(list);
	free_float_const(node);

	return 1;
}

int floatconst_test2(bool debug) {

	if(debug){
		printf("TEST: floatconst_test2\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY_ARITHMETIC,"-"));
	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list);
	
	assert(node != NULL);
	
	freeTokenList(list);
	free_float_const(node);
	
	return 1;
}
