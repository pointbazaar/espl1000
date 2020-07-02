#include "BoolConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/BoolConst.h"

#include <stdbool.h>
#include <stdio.h>

int boolconst_test_parse_bool_constant_node(bool debug)  {

	if(debug){
		printf("boolconst_test_parse_bool_constant_node\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(BCONST,"true"));

	struct BoolConst* b = makeBoolConst(list,debug);

	if(b == NULL){
		return 0;
	}

	bool assert1 = b->value;

	return (assert1)?1:0;
}

