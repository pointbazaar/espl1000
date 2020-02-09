#include <vector>

#include "BasicTypeWrapped.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "SubrType.hpp"
#include "SimpleType.hpp"

BasicTypeWrapped::BasicTypeWrapped(SimpleType* typeNode) {
	this->m1 = typeNode;
}

BasicTypeWrapped::BasicTypeWrapped(SubrType* typeNode) {
	this->m2 = typeNode;
}

BasicTypeWrapped::BasicTypeWrapped(TokenList tokens) {

	TokenList copy = tokens.copy();

	if (copy.size() > 1 && copy.get(0).tokenEquals(Token(LPARENS))) {
		TokenList copy2 = copy.copy();

		copy2.expectAndConsumeOtherWiseThrowException(Token(LPARENS));
		this->m2 = new SubrType(copy2);
		copy2.expectAndConsumeOtherWiseThrowException(Token(RPARENS));

		copy.set(copy2);

	} else {
		this->m1 = new SimpleType(copy);
	}
	tokens.set(copy);
}

