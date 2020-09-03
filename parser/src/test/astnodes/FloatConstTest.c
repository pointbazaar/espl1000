#include "FloatConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../main/parsing/FloatConst.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>

int floatconst_test1(bool debug) {

	if(debug){
		printf("TEST: floatconst_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);
	
	const bool a1 = node != NULL;
	
	freeTokenList(list);
	freeFloatConst(node);

	return (a1)?1:0;
}

int floatconst_test2(bool debug) {

	if(debug){
		printf("TEST: floatconst_test2\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY,"-"));
	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);
	
	const bool a1 = node != NULL;
	
	freeTokenList(list);
	freeFloatConst(node);
	
	return (a1)?1:0;
}
