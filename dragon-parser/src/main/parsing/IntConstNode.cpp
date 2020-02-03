
//standard headers
#include <vector>
#include <set>

//project headers
#include "IntConstNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

IntConstNode::IntConstNode(TokenList tokens) {

	TokenList copy = TokenList(tokens);

	if (copy.get(0) instanceof OperatorToken) {

		BaseToken tk = copy.get(0);
		if (tk.value.compare("-")==0 && (copy.get(1).kind == INTEGER)) {
			this->number = -(((IntegerNonNegativeConstantToken) copy.get(1)).value);
			copy.consume(2);
		} else {
			throw "cannot parse integer constant node with such operator:" + tk.operator;
		}
	} else if (copy.get(0).kind == INTEGER) {
		this->number = ((IntegerNonNegativeConstantToken) copy.get(0)).value;
		copy.consume(1);
	} else {
		throw "could not read stringConstant node";
	}
	tokens.set(copy);
}

