#include "SimpleTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/typenodes/SimpleType.h"
#include "../../main/parsing/typenodes/BasicTypeWrapped.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int simpletype_test_typenode_parsing(bool debug) {

	if(debug){
		printf("TEST: simpletype_test_typenode_parsing\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEIDENTIFIER,"MyType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped2(list, debug);

	bool assert1 = (0 == list_size(list));
	bool assert2 = node != NULL;
	
	freeTokenList(list);
	freeBasicTypeWrapped(node);

	return (assert1 && assert2)?1:0;
}


int simpletype_test_typenode_parsing_fails(bool debug) {

	if(debug){
		printf("TEST: simpletype_test_typenode_parsing_fails\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"myIllegalType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped2(list, debug);

	bool a1 = node == NULL;
	
	freeTokenList(list);
	//we do not free 'node' as it should have failed

	return (a1)?1:0;
}


int simpletype_test_typenode_parsing_anytype(bool debug) {

	if(debug){
		printf("TEST: simpletype_test_typenode_parsing_anytype\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ANYTYPE,"#"));

	struct SimpleType* node = makeSimpleType2(list, debug);

	bool assert1 = (0 == list_size(list));
	bool assert2 = node != NULL;
	
	freeTokenList(list);
	freeSimpleType(node);

	return (assert1 && assert2)?1:0;
}

int simpletype_test_generic(bool debug){

	if(debug){
		printf("TEST: simpletype_test_generic\n");
	}

	struct TokenList* l = makeTokenList();
	list_add(l, makeToken2(TYPEIDENTIFIER,"Point"));
	list_add(l, makeToken2(OPKEY,"<"));
	list_add(l, makeToken2(TPARAM,"0"));
	list_add(l, makeToken2(OPKEY,">"));

	struct SimpleType* node = makeSimpleType2(l, debug);

	assert(node != NULL);
	assert(node->typeParamCount == 1);
	assert(0 == list_size(l));
	
	freeTokenList(l);
	freeSimpleType(node);

	return 1;
}
