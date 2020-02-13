#include "FloatConstTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/FloatConst.hpp"

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
