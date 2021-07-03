#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "SimpleTypeTest.h"

#include "types/SimpleType.h"
#include "types/BasicType.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

static void status(char* msg){
	
	printf(" - [TEST] %s\n", msg);
}

int simpletype_test_typenode_parsing(bool debug) {

	status("simpletype_test_typenode_parsing");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEID,"MyType") );

	struct BasicType* node = makeBasicType2(list, debug);

	assert(0 == list_size(list));
	assert(node != NULL);
	
	freeTokenList(list);
	freeBasicType(node);

	return 1;
}


int simpletype_test_typenode_parsing_fails(bool debug) {

	status("simpletype_test_typenode_parsing_fails");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"myIllegalType") );

	struct BasicType* node = makeBasicType2(list, debug);
	assert(node == NULL);
	
	freeTokenList(list);
	//we do not free 'node' as it should have failed

	return 1;
}


int simpletype_test_typenode_parsing_anytype(bool debug) {

	status("simpletype_test_typenode_parsing_anytype");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ANYTYPE,"#"));

	struct SimpleType* node = makeSimpleType(list, debug);

	assert(0 == list_size(list));
	assert(node != NULL);
	
	freeTokenList(list);
	freeSimpleType(node);

	return 1;
}

int simpletype_test_generic(bool debug){

	status("simpletype_test_generic");

	struct TokenList* l = makeTokenList();
	
	list_add(l, makeToken2(TYPEID,"Point"));
	list_add(l, makeToken2(OPKEY_RELATIONAL,"<"));
	list_add(l, makeToken2(TPARAM,"0"));
	list_add(l, makeToken2(OPKEY_RELATIONAL,">"));

	struct SimpleType* node = makeSimpleType(l, debug);

	assert(node                != NULL);
	assert(node->structType    != NULL);
	assert(node->primitiveType == NULL);
	
	assert(node->structType->typeParamCount == 1);
	assert(0 == list_size(l));
	
	freeTokenList(l);
	freeSimpleType(node);

	return 1;
}
