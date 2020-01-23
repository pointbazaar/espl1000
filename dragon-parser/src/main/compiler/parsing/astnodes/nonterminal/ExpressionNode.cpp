
//standard headers
#include <vector>
#include <set>

//project headers
#include "ExpressionNode.hpp"

public final class ExpressionNode implements IASTNode,  ITermNode {

	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	public TermNode term1;
	public Optional<OperatorNode> op = Optional.empty();
	public Optional<TermNode> term2 = Optional.empty();


	public ExpressionNode(final TermNode term) {
		this.term1 = term;
	}

	public ExpressionNode(final TokenList tokens) throws Exception {

		// temporary containers

		final List<OperatorNode> operatorNodes = new ArrayList<>();

		final List<TermNode> termNodes = new ArrayList<>();

		// end of temporary containers

		final TokenList copy = tokens.copy();

		termNodes.add(new TermNode(copy));

		try {

			while (true) {
				final TokenList copy2 = new TokenList(copy);

				final OperatorNode myop = new OperatorNode(copy2);
				final TermNode myterm = new TermNode(copy2);

				operatorNodes.add(myop);
				termNodes.add(myterm);

				copy.set(copy2);
			}
		} catch (Exception e) {
			//pass
		}

		tokens.set(copy);

		performTreeTransformation(operatorNodes,termNodes);
	}

	public ExpressionNode(TermNode leftTerm, OperatorNode op, TermNode rightTerm) {
		this.term1=leftTerm;
		this.op=Optional.of(op);
		this.term2=Optional.of(rightTerm);
	}

	private void performTreeTransformation(
			final List<OperatorNode> ops,
			final List<TermNode> terms
	){
		//transform the list into a tree, respecting operator precedence

		/*
		Operator Precedences (lower number means higher precedence)

		3   *,/,%
		4   +,-
		5   <<,>>
		7   !=,==

		11  &&
		12  ||

		 */

		//we can construct an array list and compare indices
		List<String> operatorPrecedence = Arrays.asList(
				"*","/","%",
				"+","-",
				"<<",">>",
				"!=","==",
				"&&",
				"||"
		);

		/*
		Algorithm:

		while there are more than 2 terms then
			Find an operator with largest precedence and make a new Expression from the 2 Terms surrounding it.
			Put that new Expression back into the list.

		assign the 2 terms to this node
		 */

		while (terms.size()>2){
			final OperatorNode opWithLargestPrecedence = ops.stream().reduce((o1,o2)->{
				if(operatorPrecedence.indexOf(o1.operator)<operatorPrecedence.indexOf(o2.operator)){
					return o1;
				}else{
					return o2;
				}
			}).get();

			final int indexOfOp = ops.indexOf(opWithLargestPrecedence);

			final TermNode leftTerm = terms.get(indexOfOp);
			final TermNode rightTerm = terms.get(indexOfOp+1);

			final ExpressionNode expr = new ExpressionNode(leftTerm,opWithLargestPrecedence,rightTerm);

			//simplify
			terms.remove(leftTerm);
			terms.remove(rightTerm);
			ops.remove(opWithLargestPrecedence);

			//insert newly created expression
			terms.add(indexOfOp,new TermNode(expr));
		}

		//now only 2 terms left
		this.term1 = terms.get(0);

		//in case of only one term
		if(ops.size()>0) {
			this.op = Optional.of(ops.get(0));
			this.term2 = Optional.of(terms.get(1));
		}
	}
}
