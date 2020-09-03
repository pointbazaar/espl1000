#include "BasicTypeWrappedTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../main/parsing/typenodes/BasicTypeWrapped.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>

int basictypewrapped_test_type_parsing_simple_type(bool debug) {

	if(debug){
		printf("TEST: basictypewrapped_test_type_parsing_simple_type\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEIDENTIFIER,"PInt"));

	struct BasicTypeWrapped* b = makeBasicTypeWrapped2(list,false);

	bool assert1 = (b->simpleType != NULL && b->subrType == NULL);	//it is SimpleType
	bool assert2 = (0 == list_size(list));
	
	freeTokenList(list);
	freeBasicTypeWrapped(b);

	return (assert1&&assert2)?1:0;
}
