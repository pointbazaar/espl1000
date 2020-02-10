#include "SimpleVarTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/SimpleVar.hpp"
#include "../../main/parsing/IntConst.hpp"

int simplevar_test_parse_simple_variable() {

	TokenList list = new TokenList();
	list.add(ID,"x");

	SimpleVar* node = new SimpleVar(list);
	bool assert1 = string("x").compare(node->name)==0;

	return (assert1)?1:0;
}

int simplevar_test_parse_simple_indexed_variable() {
	TokenList list = new TokenList();
	list.add(ID,"x");
	list.add(LBRACKET);
	list.add(INTEGER,"0");
	list.add(RBRACKET);


	SimpleVar* node = new SimpleVar(list);
	bool assert1 = string("x").compare( node->name) == 0;
	bool assert2 = (node->indexOptional.has_value());

	bool assert3 = (
			0 == 
			((IntConst) node->indexOptional.value()->term1.termNode)
			.number
	);

	return (assert1&&assert2&&assert3)?1:0;
}
