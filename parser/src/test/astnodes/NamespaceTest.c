#include "NamespaceTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Namespace.h"
#include "../../main/parsing/StructDecl.h"

#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>

int namespace_test_can_parse_namespace_with_1_empty_struct(bool debug) {

	if(debug){
		printf("TEST: namespace_test_can_parse_namespace_with_1_empty_struct\n");
	}

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(STRUCT));
	list_add(l, makeToken2(TYPEIDENTIFIER,"MyStruct"));
	list_add(l, makeToken(LCURLY));
	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l,"Main", debug);

	bool assert1 = (1 == n->count_structs);
	bool assert2 = (0 == n->structs[0]->count_members);
	
	freeTokenList(l);
	freeNamespace(n);

	return (assert1&&assert2)?1:0;
}

int namespace_test_can_parse_namespace_with_1_empty_method(bool debug) {

	if(debug){
		printf("TEST: namespace_test_can_parse_namespace_with_1_empty_method\n");
	}

	//TODO:
	//these parser tests are problematic, because they don't just test the parser,
	//they also test the lexer.
	//i should write the tests so that the parser alone can accomplic shem

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken(FN));
	list_add(l, makeToken2(ID,"main"));
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken2(TYPEIDENTIFIER,"String"));
	list_add(l, makeToken2(ID,"hello"));
	list_add(l, makeToken(RPARENS));
	list_add(l, makeToken(ARROW));
	list_add(l, makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add(l, makeToken(LCURLY));

	list_add(l, makeToken(RCURLY));

	struct Namespace* n = makeNamespace(l,"Main", debug);

	bool assert1 = strcmp("Main", n->name) == 0;
	bool assert2 = (1 == n->count_methods);
	
	freeTokenList(l);
	freeNamespace(n);

	return (assert1&&assert2)?1:0;
}

