#include "FloatConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/FloatConst.h"

int floatconst_test1() {
	try {
		TokenList* list = new TokenList();
		list->add(OPKEY,"-");
		list->add(FLOATING,"4.0f");
		struct FloatConst* node = makeFloatConst(list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int floatconst_test2() {
	try {
		TokenList* list = new TokenList();
		list->add(FLOATING,"4.0f");
		struct FloatConst* node = makeFloatConst(list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}
