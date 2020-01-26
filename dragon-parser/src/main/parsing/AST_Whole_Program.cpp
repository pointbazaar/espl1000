
//standard headers
#include <vector>
#include <set>
#include <algorithm>
#include <string>

//project headers
#include "AST_Whole_Program.hpp"

using namespace std;

AST_Whole_Program::AST_Whole_Program(NamespaceNode myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node
	this.namespaceNodes.add(myNamespace);
}

AST_Whole_Program::AST_Whole_Program(TokenList tokens, string namespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		System.out.println("parse AST_Whole_Program");
	}

	this.namespaceNodes.add(new NamespaceNode(tokens, namespace, debug));
}

