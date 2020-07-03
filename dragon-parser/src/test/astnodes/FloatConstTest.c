#include "FloatConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/FloatConst.h"

#include <stdio.h>

int floatconst_test1(bool debug) {

	if(debug){
		printf("floatconst_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);

	return (node != NULL)?1:0;
}

int floatconst_test2(bool debug) {

	if(debug){
		printf("floatconst_test2\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY,"-"));
	list_add(list, makeToken2(FLOATING,"4.0"));

	struct FloatConst* node = makeFloatConst(list,debug);
	
	return (node!=NULL)?1:0;
}
