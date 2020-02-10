#include <vector>
#include <string>
#include <iostream>

#include "Namespace.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

Namespace::Namespace(
		TokenList* tokens,
		string name,
		bool debug
) {

	if (debug) {
		cout << "Namespace(...)" << endl;
		cout << "from: " + tokens->code() << endl;
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
	
	if (copy->size() > 0) {

		Token next_subr = copy->get(0);
		cout << next_subr.kind << endl;

		while (next_subr.kind == FN) {
			this->methods.push_back(Method(*copy, debug));
			if (copy->size() > 0) {
				next_subr = copy->get(0);
			} else {
				break;
			}
		}
	}

	if(debug){
		cout << "done parsing Namespace Node" << endl;
	}

	tokens->set(*copy);
}

