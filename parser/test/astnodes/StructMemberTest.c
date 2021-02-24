#include "StructMemberTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/struct/StructMember.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int structmember_test_can_parse_struct_member(bool debug) {

	if(debug){
		printf("TEST: structmember_test_can_parse_struct_member\n");
	}

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

	struct StructMember* node = makeStructMember(list,debug);

	assert(node != NULL);

	assert( strcmp("subr", node->name) == 0);
	assert(0 == list_size(list));
	
	freeTokenList(list);
	freeStructMember(node);

	return 1;
}
