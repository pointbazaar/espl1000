#include "SimpleTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/typenodes/SimpleType.h"
#include "../../main/parsing/typenodes/BasicTypeWrapped.h"

#include <stdbool.h>

int simpletype_test_typenode_parsing() {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEIDENTIFIER,"MyType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped(list);
	bool assert1 = (0 == list_size(list));

	return (assert1)?1:0;
}


int simpletype_test_typenode_parsing_fails() {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"myIllegalType") );

	struct BasicTypeWrapped* node = makeBasicTypeWrapped(list);
	return (node!=NULL)?1:0;
}


int simpletype_test_typenode_parsing_anytype() {

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(ANYTYPE) );

	struct SimpleType* node = makeSimpleType(list_code(list));
	bool assert1 = (0 == list_size(list));

	return (assert1)?1:0;
}
