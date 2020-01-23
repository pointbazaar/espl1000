
//standard headers
#include <vector>
#include <set>

//project headers
#include "IntConstNode.hpp"

public final class IntConstNode implements IASTNode, ITermNode {

	//this can also have a negative value if it is preceded by a '-' operator token
	public final int number;

	public IntConstNode(final TokenList tokens) throws Exception {

		TokenList copy = new TokenList(tokens);

		if (copy.get(0) instanceof OperatorToken) {
			OperatorToken tk = (OperatorToken) copy.get(0);
			if (tk.operator.equals("-") && (copy.get(1) instanceof IntegerNonNegativeConstantToken)) {
				this.number = -(((IntegerNonNegativeConstantToken) copy.get(1)).value);
				copy.consume(2);
			} else {
				throw new Exception("cannot parse integer constant node with such operator:" + tk.operator);
			}
		} else if (copy.get(0) instanceof IntegerNonNegativeConstantToken) {
			this.number = ((IntegerNonNegativeConstantToken) copy.get(0)).value;
			copy.consume(1);
		} else {
			throw new Exception("could not read stringConstant node");
		}
		tokens.set(copy);
	}

}
