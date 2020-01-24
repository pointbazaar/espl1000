
//standard headers
#include <vector>
#include <set>

//project headers
#include "StructDeclNode.hpp"

class StructDeclNode : IASTNode{

public:
	SimpleTypeNode typeNode;
	vector<StructMemberDeclNode> members;

	StructDeclNode(TokenList tokens, bool debug) throws Exception {

		if (debug) {
			System.out.println("try to parse " + this.getClass().getSimpleName() + " from '" + tokens.toSourceCodeFragment() + "'");
		}

		TokenList copy = tokens.copy();

		copy.expectAndConsumeOtherWiseThrowException(new StructToken());

		this.typeNode = new SimpleTypeNode(copy);

		copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

		// parse struct members
		IToken next = copy.get(0);
		while (!(next instanceof RCurlyToken)) {

			if (this.members.size() > 0) {
				copy.expectAndConsumeOtherWiseThrowException(new CommaToken());
			}

			this.members.add(new StructMemberDeclNode(copy, debug));
			next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());

		tokens.set(copy);
	}

};