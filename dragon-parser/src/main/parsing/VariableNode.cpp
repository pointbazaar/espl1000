
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <map>

//project headers
#include "VariableNode.hpp"
#include "ITermNode.hpp"
#include "SimpleVariableNode.hpp"
#include "../commandline/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/TokenList.hpp"

VariableNode::VariableNode(TokenList tokens) {
	TokenList copy = tokens.copy();

	this->simpleVariableNode = SimpleVariableNode(copy);

	if (copy.size() > 0) {
		BaseToken next = copy.get(0);
		while (next.kind == STRUCTMEMBERACCESS) {

			copy.expectAndConsumeOtherWiseThrowException(BaseToken(STRUCTMEMBERACCESS));
			this->memberAccessList.push_back(VariableNode(copy));
			if (copy.size() > 0) {
				next = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens.set(copy);
}

