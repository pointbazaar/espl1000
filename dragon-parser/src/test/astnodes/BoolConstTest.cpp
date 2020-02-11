#include "BoolConstTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/BoolConst.hpp"

int boolconst_test_parse_bool_constant_node()  {
	TokenList* list = new TokenList();
	
	list->add(BCONST,"true");

	BoolConst* b = makeBoolConst(*list,false);
	bool assert1 = ( b->boolValue);

	return (assert1)?1:0;
}

