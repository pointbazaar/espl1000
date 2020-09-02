#include "StructMemberTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/StructMember.h"
#include "../../../../ast/free_ast.h"

#include <string.h>
#include <stdio.h>

int structmember_test_can_parse_struct_member(bool debug) {

	if(debug){
		printf("TEST: structmember_test_can_parse_struct_member\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(LPARENS,"("));
		list_add(list, makeToken2(TYPEIDENTIFIER,"PInt") );
		list_add(list, makeToken2(RPARENS,")"));
		list_add(list, makeToken2(ARROW,"->"));
		list_add(list, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(list, makeToken2(RPARENS,")"));

	list_add(list, makeToken2(ID,"subr"));

	struct StructMember* node = makeStructMember(list,debug);

	if(node == NULL){return 0;}

	bool assert1 = strcmp("subr", node->name) == 0;
	bool assert2 = (0 == list_size(list));
	
	freeTokenList(list);
	freeStructMember(node);

	return (assert1&&assert2)?1:0;
}
