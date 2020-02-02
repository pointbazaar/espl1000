
//standard headers
#include <vector>
#include <string>

//project headers
#include "StructDeclNode.hpp"
#include "../lexing/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

StructDeclNode(TokenList tokens, bool debug) {

	if (debug) {
		cout << "try to parse from '" + tokens.toSourceCodeFragment() + "'" << endl;
	}

	TokenList copy = tokens.copy();

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(STRUCT));

	this.typeNode = new SimpleTypeNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	// parse struct members
	BaseToken next = copy.get(0);
	while (!(next->kind == RCURLY)) {

		if (this.members.size() > 0) {
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(COMMA));
		}

		this.members.add(new StructMemberDeclNode(copy, debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	tokens.set(copy);
}

