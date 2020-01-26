
//standard headers
#include <vector>
#include <string>

//project headers
#include "NamespaceNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

NamespaceNode::NamespaceNode(
		TokenList tokens,
		string namespace,
		bool debug
) {

	if (debug) {
		cout << "try to parse from " + tokens.toSourceCodeFragment() << endl;
	}

	this.srcPath = "/dev/null";
	this.name = namespace;
	TokenList copy = tokens.copy();

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

	//it is be nice to have prefix 'fn' before a function
	//to make parsing easier.
	//this does not add much boilerplate to the syntax
	//and would probably make the parser faster
	if (copy.size() > 0) {

		IToken next_subr = copy.get(0);

		while (next_subr instanceof FnToken) {
			this.methods.add(new MethodNode(copy, debug));

			if (copy.size() > 0) {
				next_subr = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens.set(copy);
}

