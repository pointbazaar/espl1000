#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "StructDeclTest.h"

#include "struct/StructDecl.h"
#include "struct/StructMember.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int structdecl_test_can_parse_empty_struct_decl() {

	status_test("structdecl_test_can_parse_empty_struct_decl");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(STRUCT));
	list_add(list, makeToken2(TYPEID, "MyStruct"));
	list_add(list, makeToken(LCURLY));
	list_add(list, makeToken(RCURLY));

	struct StructDecl* s = makeStructDecl(list);

	assert(0 == s->count_members);

	freeTokenList(list);
	free_struct_decl(s);

	return 1;
}

int structdecl_test_can_parse_struct_with_1_member() {

	status_test("structdecl_test_can_parse_struct_with_1_member");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(STRUCT));
	list_add(list, makeToken2(TYPEID, "MyStruct"));
	list_add(list, makeToken(LCURLY));
	list_add(list, makeToken2(TYPEID, "uint"));
	list_add(list, makeToken2(ID, "a"));
	list_add(list, makeToken(SEMICOLON)),
	    list_add(list, makeToken(RCURLY));

	struct StructDecl* node = makeStructDecl(list);

	assert(1 == node->count_members);
	assert(strcmp("a", node->members[0]->name) == 0);

	freeTokenList(list);
	free_struct_decl(node);

	return 1;
}

int structdecl_test_can_parse_struct_with_2_members() {

	status_test("structdecl_test_can_parse_struct_with_2_members");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(STRUCT));
	list_add(list, makeToken2(TYPEID, "MyStruct"));
	list_add(list, makeToken(LCURLY));
	list_add(list, makeToken2(TYPEID, "uint"));
	list_add(list, makeToken2(ID, "a"));
	list_add(list, makeToken(SEMICOLON)),
	    list_add(list, makeToken2(TYPEID, "uint"));
	list_add(list, makeToken2(ID, "b"));
	list_add(list, makeToken(SEMICOLON)),
	    list_add(list, makeToken(RCURLY));

	struct StructDecl* node = makeStructDecl(list);

	assert(2 == node->count_members);

	assert(strcmp("a", node->members[0]->name) == 0);

	assert(strcmp("b", node->members[1]->name) == 0);

	freeTokenList(list);
	free_struct_decl(node);

	return 1;
}
