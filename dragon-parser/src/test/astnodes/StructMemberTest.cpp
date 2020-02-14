#include "StructMemberTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/StructMember.hpp"

int structmember_test_can_parse_struct_member() {

	TokenList list = TokenList();

	list.add(LPARENS);
	list.add(RPARENS);
	list.add(TYPEIDENTIFIER,"PInt");
	list.add(RPARENS);
	list.add(ARROW);
	list.add(TYPEIDENTIFIER,"PInt");
	list.add(RPARENS);
	list.add(ID,"subr");

	struct StructMember* node = makeStructMember(&list,false);

	bool assert1 = string("subr").compare( node->name) == 0;
	bool assert2 = (0 == list.size());

	return (assert1&&assert2)?1:0;
}
