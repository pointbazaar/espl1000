
//standard headers
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

//project headers
#include "AST_Whole_Program.hpp"

using namespace std;

AST_Whole_Program::AST_Whole_Program(NamespaceNode myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node
	this->namespaceNodes.push_back(myNamespace);
}

AST_Whole_Program::AST_Whole_Program(TokenList tokens, string myNamespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		cout << "parse AST_Whole_Program" << endl;
	}

	this->namespaceNodes.push_back(NamespaceNode(tokens, myNamespace, debug));
}

