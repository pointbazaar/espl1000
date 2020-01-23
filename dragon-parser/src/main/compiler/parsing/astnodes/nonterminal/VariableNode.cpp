
//standard headers
#include <vector>
#include <set>

//project headers
#include "VariableNode.hpp"

public final class VariableNode implements IASTNode, ITermNode {

	public final SimpleVariableNode simpleVariableNode;
	public final List<VariableNode> memberAccessList = new ArrayList<>();

	public VariableNode(TokenList tokens) throws Exception {
		final TokenList copy = tokens.copy();

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

}
