
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <map>

//project headers
#include "VariableNode.hpp"
#include "ITermNode.hpp"
#include "SimpleVariableNode.hpp"
#include "../lexing/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../lexing/TokenList.hpp"

VariableNode::VariableNode(TokenList tokens) {
	TokenList copy = tokens.copy();

	this->simpleVariableNode = SimpleVariableNode(copy);

	if (copy.size() > 0) {
		BaseToken next = copy.get(0);
		while (next.kind == STRUCTMEMBERACCESS) {

			copy.expectAndConsumeOtherWiseThrowException(BaseToken(STRUCTMEMBERACCESS));
			this.memberAccessList.add(VariableNode(copy));
			if (copy.size() > 0) {
				next = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens.set(copy);
}

