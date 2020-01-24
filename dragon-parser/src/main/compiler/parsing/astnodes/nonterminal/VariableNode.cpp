
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <algorithm>

//project headers
#include "VariableNode.hpp"

class VariableNode : IASTNode, ITermNode {

public:
	
	SimpleVariableNode simpleVariableNode;
	vector<VariableNode> memberAccessList;

	VariableNode(TokenList tokens) throws Exception {
		TokenList copy = tokens.copy();

		this.simpleVariableNode = new SimpleVariableNode(copy);

		if (copy.size() > 0) {
			IToken next = copy.get(0);
			while (next instanceof StructMemberAccessToken) {

				copy.expectAndConsumeOtherWiseThrowException(new StructMemberAccessToken());
				this.memberAccessList.add(new VariableNode(copy));
				if (copy.size() > 0) {
					next = copy.get(0);
				} else {
					break;
				}
			}
		}

		tokens.set(copy);
	}

};