#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "SimpleTypeTest.h"
#include "ast/util/free_ast.h"
#include "parser/test/test_parser_util.h"
#include "token/TokenKeys.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"
#include "types/BasicType.h"
#include "types/SimpleType.h"

int simpletype_test_typenode_parsing() {

	status_test("simpletype_test_typenode_parsing");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(TYPEID, "MyType"));

	struct BasicType* node = makeBasicType2(list);

	assert(0 == list_size(list));
	assert(node != NULL);

	freeTokenList(list);
	free_basic_type(node);

	return 1;
}

int simpletype_test_typenode_parsing_fails() {

	status_test("simpletype_test_typenode_parsing_fails");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID, "myIllegalType"));

	struct BasicType* node = makeBasicType2(list);
	assert(node == NULL);

	freeTokenList(list);
	// we do not free 'node' as it should have failed

	return 1;
}

int simpletype_test_typenode_parsing_anytype() {

	status_test("simpletype_test_typenode_parsing_anytype");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ANYTYPE, "#"));

	struct SimpleType* node = makeSimpleType(list);

	assert(0 == list_size(list));
	assert(node != NULL);

	freeTokenList(list);
	free_simple_type(node);

	return 1;
}
