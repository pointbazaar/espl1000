#include "BoolConstTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/BoolConst.hpp"

int boolconst_test_parse_bool_constant_node(bool debug)  {
	TokenList* list = new TokenList();
	
	list->add(BCONST,"true");

	struct BoolConst* b = makeBoolConst(list,debug);
	bool assert1 = ( b->value);

	return (assert1)?1:0;
}

