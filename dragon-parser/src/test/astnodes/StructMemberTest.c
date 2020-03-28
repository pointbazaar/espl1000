#include "StructMemberTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/StructMember.h"

#include <string.h>

int structmember_test_can_parse_struct_member() {

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(LPARENS) );
	list_add(list, makeToken(RPARENS) );
	list_add(list, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(list, makeToken(RPARENS) );
	list_add(list, makeToken(ARROW) );
	list_add(list, makeToken2(TYPEIDENTIFIER,"PInt") );
	list_add(list, makeToken(RPARENS) );
	list_add(list, makeToken2(ID,"subr") );

	struct StructMember* node = makeStructMember(list,false);

	bool assert1 = strcmp("subr", node->name) == 0;
	bool assert2 = (0 == list_size(list));

	return (assert1&&assert2)?1:0;
}
