#include "SimpleTypeTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/typenodes/SimpleType.hpp"
#include "../../main/parsing/typenodes/BasicTypeWrapped.hpp"

int simpletype_test_typenode_parsing() {

	TokenList list = TokenList();
	list.add(TYPEIDENTIFIER,"MyType");

	struct BasicTypeWrapped* node = makeBasicTypeWrapped(&list,false);
	bool assert1 = (0 == list.size());

	return (assert1)?1:0;
}


int simpletype_test_typenode_parsing_fails() {

	TokenList list = TokenList();
	list.add(ID,"myIllegalType");

	try {
		struct BasicTypeWrapped* node = makeBasicTypeWrapped(&list,false);
		return 0;
	} catch (string e) {
		return 1;
	}
}


int simpletype_test_typenode_parsing_anytype() {

	TokenList list = TokenList();
	list.add(ANYTYPE);

	struct SimpleType* node = makeSimpleType(&list,false);
	bool assert1 = (0 == list.size());

	return (assert1)?1:0;
}
