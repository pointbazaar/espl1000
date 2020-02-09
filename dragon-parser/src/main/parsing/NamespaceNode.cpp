
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
		TokenList* tokens,
		string name,
		bool debug
) {

	if (debug) {
		cout << "NamespaceNode(...)" << endl;
		cout << "from: " + tokens->toSourceCodeFragment() << endl;
		cout << tokens->size() << endl;
	}

	this->srcPath = "/dev/null";
	this->name = name;
	TokenList copy_1 = tokens->copy();
	TokenList* copy = &copy_1;

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
	cout << copy->size() << endl;
	if (copy->size() > 0) {

		BaseToken next_subr = copy->get(0);
		cout << next_subr.kind << endl;

		while (next_subr.kind == FN) {
			this->methods.push_back(MethodNode(*copy, debug));
			if (copy->size() > 0) {
				next_subr = copy->get(0);
			} else {
				break;
			}
		}
	}

	if(debug){
		cout << "done parsing NamespaceNode" << endl;
	}

	tokens->set(*copy);
}

