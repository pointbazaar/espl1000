
//standard headers
#include <vector>
#include <set>

//project headers
#include "SubroutineTypeNode.hpp"

using namespace std;

SubroutineTypeNode::SubroutineTypeNode(TypeNode return_type, final boolean hasSideEffects) {
	this.returnType = return_type;
	this.hasSideEffects = hasSideEffects;
}

SubroutineTypeNode::SubroutineTypeNode(TokenList tokens) throws Exception {

	TokenList copy = tokens.copy();

	copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

	boolean sucess_argument_types = true;
	try {
		this.argumentTypes.add(new TypeNode(copy));
	} catch (string e) {
		sucess_argument_types = false;
	}
	while (sucess_argument_types) {
		try {
			TokenList copy2 = copy.copy();

			copy2.expectAndConsumeOtherWiseThrowException(new CommaToken());
			this.argumentTypes.add(new TypeNode(copy2));

			copy.set(copy2);
		} catch (string e) {
			sucess_argument_types = false;
		}
	}

	copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

	if (copy.head() instanceof ArrowToken) {
		ArrowToken arrow = (ArrowToken) copy.head();
		this.hasSideEffects = !arrow.is_functional;
		copy.consume(1);
	} else {
		throw "expected an arrow token here";
	}

	this.returnType = new TypeNode(copy);

	tokens.set(copy);
}

