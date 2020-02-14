#include "VariableTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/Variable.hpp"
#include "../../main/parsing/SimpleVar.hpp"

#include <iostream>

int variable_test_parse_struct_member_access(bool debug) {

	if(debug){
		cout << "variable_test_parse_struct_member_access(...)" << endl;
	}

	TokenList tokens = TokenList();
	tokens.add(ID,"x");
	tokens.add(STRUCTMEMBERACCESS,".");
	tokens.add(ID,"a");

	struct Variable* v = makeVariable(&tokens,debug);

	bool assert1 = (1 == v->count_memberAccessList);
	bool assert2 = string("x").compare( v->simpleVariableNode->name)==0;
	bool assert3 = !(v->simpleVariableNode->indexOptional != NULL);
	bool assert4 = string("a").compare( v->memberAccessList[0]->simpleVariableNode->name);
	bool assert5 = !(v->memberAccessList[0]->simpleVariableNode->indexOptional != NULL);

	return (assert1&&assert2&&assert3&&assert4&&assert5)?1:0;
}

int variable_test_parse_index_access(bool debug) {
	if(debug){
		cout << "variable_test_parse_index_access(...)" << endl;
	}

	TokenList tokens = TokenList();
	tokens.add(ID,"x");
	tokens.add(LBRACKET);
	tokens.add(INTEGER,"0");
	tokens.add(RBRACKET);

	struct Variable* node = makeVariable(&tokens,debug);

	bool assert1 = (0 == node->count_memberAccessList);
	bool assert2 = string("x").compare(node->simpleVariableNode->name) == 0;
	bool assert3 = (node->simpleVariableNode->indexOptional != NULL);

	return (assert1&&assert2&&assert3)?1:0;
}

int variable_test_parse_struct_member_access_and_index_access(bool debug) {

	if(debug){
		cout << "variable_test_parse_struct_member_access_and_index_access(...)" << endl;
	}

	// x.a[0].b
	TokenList tokens = TokenList();
	tokens.add(ID,"x");
	tokens.add(STRUCTMEMBERACCESS);
	tokens.add(ID,"a");

	tokens.add(LBRACKET);
	tokens.add(INTEGER,"0");
	tokens.add(RBRACKET);

	tokens.add(STRUCTMEMBERACCESS);
	tokens.add(ID,"b");

	struct Variable* node = makeVariable(&tokens,debug);

	bool assert1 = (1 == node->count_memberAccessList);
	bool assert2 = string("x").compare(node->simpleVariableNode->name) == 0;

	bool assert3 = string("a").compare( node->memberAccessList[0]->simpleVariableNode->name) == 0;
	bool assert4 = (node->memberAccessList[0]->simpleVariableNode->indexOptional != NULL);

	return (assert1&&assert2&&assert3&&assert4)?1:0;
}
