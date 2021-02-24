#include "FloatConstTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/astnodes/const/FloatConst.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>
#include <assert.h>

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
