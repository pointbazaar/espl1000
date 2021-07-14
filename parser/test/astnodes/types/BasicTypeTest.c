#include <stdio.h>
#include <assert.h>

#include "BasicTypeTest.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "types/BasicType.h"

#include "ast/util/free_ast.h"

int basictype_test_type_parsing_simple_type(bool debug) {

	if(debug){
		printf("TEST: basictype_test_type_parsing_simple_type\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEID,"Carrot"));

	struct BasicType* b = makeBasicType2(list);

	//it is SimpleType
	assert(b->simple_type != NULL && b->subr_type == NULL);
	
	list_print(list);
	assert(0 == list_size(list));
	
	freeTokenList(list);
	free_basic_type(b);

	return 1;
}
