
//standard headers
#include <vector>
#include <set>

//project headers
#include "IdentifierNode.hpp"

class IdentifierNode : IASTNode {

public:
	//this identifies a variable
	string identifier;

	IdentifierNode(TokenList tokens) throws Exception {

		IToken token = tokens.get(0);

		if (token instanceof IdentifierToken) {
			this.identifier = token.getContents();
			tokens.consume(1);

		} else {
			String msg = "could not read identifier from token: " + token.getContents();
			msg += " in " + tokens.relPath + ":" + token.getLineNumber();
			msg += " in context '" + tokens.toSourceCodeFragment().substring(0, Math.min(20, tokens.toSourceCodeFragment().length())) + "'";

			throw new Exception(msg);
		}

	}

};