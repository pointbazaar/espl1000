#include "SimpleTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/typenodes/SimpleType.h"
#include "../../main/parsing/typenodes/BasicTypeWrapped.h"

#include <stdbool.h>

int simpletype_test_typenode_parsing(bool debug) {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEIDENTIFIER,"MyType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped2(list, false);

	bool assert1 = (0 == list_size(list));
	bool assert2 = node != NULL;

	return (assert1 && assert2)?1:0;
}


int simpletype_test_typenode_parsing_fails(bool debug) {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"myIllegalType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped2(list, false);

	return (node!=NULL)?1:0;
}


int simpletype_test_typenode_parsing_anytype(bool debug) {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(ANYTYPE) );

	struct SimpleType* node = makeSimpleType(list_code(list, false));

	bool assert1 = (0 == list_size(list));
	bool assert2 = node != NULL;

	return (assert1 && assert2)?1:0;
}
