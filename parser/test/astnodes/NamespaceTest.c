#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "NamespaceTest.h"

#include "Namespace.h"
#include "struct/StructDecl.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int namespace_test_can_parse_namespace_with_1_empty_struct(bool debug) {

	if(debug){
		printf("TEST: namespace_test_can_parse_namespace_with_1_empty_struct\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(STRUCT));
	list_add(l, makeToken2(TYPEID,"MyStruct"));
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l,"Main", debug);

	assert(1 == n->count_structs);
	assert(0 == n->structs[0]->count_members);
	
	freeTokenList(l);
	freeNamespace(n);

	return 1;
}

int namespace_test_can_parse_namespace_with_1_empty_method(bool debug) {

	if(debug){
		printf("TEST: namespace_test_can_parse_namespace_with_1_empty_method\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(FN));
	list_add(l, makeToken2(ID,"main"));
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken(RPARENS));
	list_add(l, makeToken(ARROW));
	list_add(l, makeToken2(TYPEID,"uint"));
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l,"Main", debug);

	assert(strcmp("Main", n->name) == 0);
	assert(1 == n->count_methods);
	
	freeTokenList(l);
	freeNamespace(n);

	return 1;
}

