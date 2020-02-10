#include "BasicTypeWrappedTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/typenodes/BasicTypeWrapped.hpp"

int basictypewrapped_test_type_parsing_simple_type() {

	TokenList list = new TokenList();
	list.add(TYPEIDENTIFIER,"PInt");

	BasicTypeWrapped* b = new BasicTypeWrapped(list);

	//TODO:
	bool assert1 = (b->typeNode instanceof SimpleTypeNode);
	bool assert2 = (0 == list.size());

	return (assert1&&assert2)?1:0;
}
