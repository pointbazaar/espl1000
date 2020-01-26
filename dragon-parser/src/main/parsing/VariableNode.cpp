
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <map>

//project headers
#include "VariableNode.hpp"
#include "ITermNode.hpp"
#include "StructMemberAccessToken.hpp"
#include "SimpleVariableNode.hpp"
#include "IToken.hpp"

	VariableNode(TokenList tokens) {
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