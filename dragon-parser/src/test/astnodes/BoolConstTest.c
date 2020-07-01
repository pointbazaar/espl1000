#include "BoolConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/BoolConst.h"

int boolconst_test_parse_bool_constant_node(bool debug)  {
	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(BCONST,"true"));

	struct BoolConst* b = makeBoolConst(list,debug);
	bool assert1 = ( b->value);

	return (assert1)?1:0;
}

