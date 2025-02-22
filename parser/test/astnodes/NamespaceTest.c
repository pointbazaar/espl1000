#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "NamespaceTest.h"

#include "Namespace.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int namespace_test_can_parse_namespace_with_1_empty_struct() {

	status_test("namespace_test_can_parse_namespace_with_1_empty_struct");

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(STRUCT));
	list_add(l, makeToken2(TYPEID, "MyStruct"));
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l, "Main");

	assert(n);

	assert(1 == n->count_structs);
	assert(0 == n->structs[0]->count_members);

	freeTokenList(l);
	free_namespace(n);

	return 1;
}

int namespace_test_can_parse_namespace_with_1_empty_method() {

	status_test("namespace_test_can_parse_namespace_with_1_empty_method");

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(FN));
	list_add(l, makeToken2(ID, "main"));
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken(RPARENS));
	list_add(l, makeToken(ARROW_SIDE_EFFECT));
	list_add(l, makeToken2(TYPEID, "uint"));
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l, "Main");

	assert(strcmp("Main", n->name) == 0);
	assert(1 == n->count_methods);

	freeTokenList(l);
	free_namespace(n);

	return 1;
}
