#include "ArrayConstTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/ArrayConst.hpp"

int arrayconst_test_empty_array() {

	TokenList list = new TokenList();

	list.add(LBRACKET);
	list.add(RBRACKET);
	ArrayConst* a = new ArrayConst(list);

	bool assert1 = (0 == a->elements.size());

	return (assert1)?1:0;
}

int arrayconst_test_simple_array() {

	TokenList list = new TokenList();

	list.add(LBRACKET);
	list.add(INTEGER,1);
	list.add(RBRACKET);

	ArrayConst* a = new ArrayConst(list);

	bool assert1 = (1 == a->elements.size());

	return (assert1)?1:0;
}

int arrayconst_test_array_multiple_elements() {

	TokenList list = new TokenList();

	list.add(LBRACKET);
	list.add(INTEGER,1);
	list.add(COMMA);
	list.add(INTEGER,2);
	list.add(RBRACKET);

	ArrayConst* a = new ArrayConst(list);

	bool assert1 = (2 == a->elements.size());

	return (assert1)?1:0;
}
