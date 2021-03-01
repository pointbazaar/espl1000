#include <stdio.h>
#include <assert.h>

#include "FloatConstTest.h"
#include "const/FloatConst.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int floatconst_test1(bool debug) {

	if(debug){
		printf("TEST: floatconst_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);
	assert(node != NULL);
	
	freeTokenList(list);
	freeFloatConst(node);

	return 1;
}

int floatconst_test2(bool debug) {

	if(debug){
		printf("TEST: floatconst_test2\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY,"-"));
	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);
	
	assert(node != NULL);
	
	freeTokenList(list);
	freeFloatConst(node);
	
	return 1;
}
