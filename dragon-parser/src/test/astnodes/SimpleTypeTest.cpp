#include "SimpleTypeTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/typenodes/SimpleType.hpp"
#include "../../main/parsing/typenodes/BasicTypeWrapped.hpp"

int simpletype_test_typenode_parsing() {

	TokenList list = new TokenList();
	list.add(TYPEIDENTIFIER,"MyType");

	BasicTypeWrapped* node = new BasicTypeWrapped(list);
	bool assert1 = (0 == list.size());

	return (assert1)?1:0;
}


int simpletype_test_typenode_parsing_fails() {

	TokenList list = new TokenList();
	list.add(ID,"myIllegalType");

	try {
		BasicTypeWrapped* node = new BasicTypeWrapped(list);
		return 0;
	} catch (string e) {
		return 1;
	}
}


int simpletype_test_typenode_parsing_anytype() {

	TokenList list = new TokenList();
	list.add(ANYTYPE);

	SimpleType* node = new SimpleType(list);
	bool assert1 = (0 == list.size());

	return (assert1)?1:0;
}
