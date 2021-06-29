#include <stdio.h>
#include <assert.h>

#include "BasicTypeWrappedTest.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "types/BasicTypeWrapped.h"

#include "ast/util/free_ast.h"

int basictypewrapped_test_type_parsing_simple_type(bool debug) {

	if(debug){
		printf("TEST: basictypewrapped_test_type_parsing_simple_type\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEID,"Carrot"));

	struct BasicTypeWrapped* b = makeBasicTypeWrapped2(list, debug);

	//it is SimpleType
	assert(b->simpleType != NULL && b->subrType == NULL);	
	
	list_print(list);
	assert(0 == list_size(list));
	
	freeTokenList(list);
	freeBasicTypeWrapped(b);

	return 1;
}
