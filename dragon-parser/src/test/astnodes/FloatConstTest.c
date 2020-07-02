#include "FloatConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/FloatConst.h"

int floatconst_test1(bool debug) {

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(OPKEY,"-"));
	list_add(list, makeToken2(FLOATING,"4.0f"));

	struct FloatConst* node = makeFloatConst(list,false);

	return (node != NULL)?1:0;
}

int floatconst_test2(bool debug) {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(FLOATING,"4.0f"));
	struct FloatConst* node = makeFloatConst(list,false);
	
	return (node!=NULL)?1:0;
}
