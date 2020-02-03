
//standard headers
#include <vector>
#include <set>

//project headers
#include "IntConstNode.hpp"
#include "../lexing/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

IntConstNode::IntConstNode(TokenList tokens) {

	TokenList copy = TokenList(tokens);

	if (copy.get(0).kind == OPERATOR) {

		BaseToken tk = copy.get(0);
		if (tk.value.compare("-")==0 && (copy.get(1).kind == INTEGER)) {
			this->number = - stoi( copy.get(1).value );
			copy.consume(2);
		} else {
			throw "cannot parse integer constant node with such operator:" + (string)tk.value;
		}
	} else if (copy.get(0).kind == INTEGER) {
		this->number = stoi( copy.get(0).value);
		copy.consume(1);
	} else {
		throw "could not read stringConstant node";
	}
	tokens.set(copy);
}

