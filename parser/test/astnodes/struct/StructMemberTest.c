#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "StructMemberTest.h"

#include "struct/StructMember.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int structmember_test_can_parse_struct_member() {

	status_test("structmember_test_can_parse_struct_member");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(TYPEID,"MyStruct") );
		list_add(list, makeToken2(RPARENS,")"));
		list_add(list, makeToken2(ARROW,"->"));
		list_add(list, makeToken2(TYPEID,"Carrot") );
	list_add(list, makeToken2(RPARENS,")"));

	list_add(list, makeToken2(ID,"subr"));
	list_add(list, makeToken(SEMICOLON));

	struct StructMember* node = makeStructMember(list);

	assert(node != NULL);

	assert( strcmp("subr", node->name) == 0);
	assert(0 == list_size(list));
	
	freeTokenList(list);
	free_struct_member(node);

	return 1;
}
