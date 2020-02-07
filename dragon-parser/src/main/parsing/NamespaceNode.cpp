
//standard headers
#include <vector>
#include <string>
#include <iostream>

//project headers
#include "NamespaceNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

NamespaceNode::NamespaceNode(
		TokenList tokens,
		string name,
		bool debug
) {

	if (debug) {
		cout << "try to parse NamespaceNode" << endl;
		cout << "try to parse from " + tokens.toSourceCodeFragment() << endl;
	}

	this->srcPath = "/dev/null";
	this->name = name;
	TokenList copy = tokens.copy();

	//TODO: add them in back later
	/*
	if(copy.size()>0) {

		IToken next_struct = copy.get(0);

		while (next_struct instanceof StructToken) {
			this.structs.add(new StructDeclNode(copy, debug));

			if (copy.size() > 0) {
				next_struct = copy.get(0);
			} else {
				break;
			}
		}
	}
	*/

	//it is be nice to have prefix 'fn' before a function
	//to make parsing easier.
	//this does not add much boilerplate to the syntax
	//and would probably make the parser faster
	if (copy.size() > 0) {

		BaseToken next_subr = copy.get(0);

		while (next_subr.kind == FN) {
			this->methods.push_back(MethodNode(copy, debug));
			if (copy.size() > 0) {
				next_subr = copy.get(0);
			} else {
				break;
			}
		}
	}

	if(debug){
		cout << "done parsing NamespaceNode" << endl;
	}

	tokens.set(copy);
}

