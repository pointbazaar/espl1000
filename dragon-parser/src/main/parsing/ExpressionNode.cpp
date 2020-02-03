
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <string>

//project headers
#include "ExpressionNode.hpp"
#include "ITermNode.hpp"

ExpressionNode::ExpressionNode(TermNode term) {
	this->term1 = term;
}

ExpressionNode::ExpressionNode(TokenList tokens) {

	// temporary containers
	vector<OperatorNode> operatorNodes;
	vector<TermNode> termNodes;
	// end of temporary containers
	TokenList copy = tokens.copy();
	termNodes.push_back(TermNode(copy));
	try {

		while (true) {
			TokenList copy2 = TokenList(copy);

			OperatorNode myop = OperatorNode(copy2);
			TermNode myterm = TermNode(copy2);

			operatorNodes.push_back(myop);
			termNodes.push_back(myterm);

			copy.set(copy2);
		}
	} catch (Exception e) {
		//pass
	}

	tokens.set(copy);

	performTreeTransformation(operatorNodes,termNodes);
}

ExpressionNode::ExpressionNode(TermNode leftTerm, OperatorNode op, TermNode rightTerm) {
	this->term1=leftTerm;
	this->op=Optional.of(op);
	this->term2=Optional.of(rightTerm);
}

void performTreeTransformation(
		vector<OperatorNode> ops,
		vector<TermNode> terms
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
	vector<string> operatorPrecedence = {
			"*","/","%",
			"+","-",
			"<<",">>",
			"!=","==",
			"&&",
			"||"
	};

	/*
	Algorithm:

	while there are more than 2 terms then
		Find an operator with largest precedence and make a new Expression from the 2 Terms surrounding it.
		Put that new Expression back into the list.

	assign the 2 terms to this node
	 */

	while (terms.size()>2){
		OperatorNode opWithLargestPrecedence = ops.stream().reduce((o1,o2)->{
			if(operatorPrecedence.indexOf(o1.operator)<operatorPrecedence.indexOf(o2.operator)){
				return o1;
			}else{
				return o2;
			}
		}).get();

		int indexOfOp = ops.indexOf(opWithLargestPrecedence);

		TermNode leftTerm = terms.at(indexOfOp);
		TermNode rightTerm = terms.at(indexOfOp+1);

		ExpressionNode expr = ExpressionNode(leftTerm,opWithLargestPrecedence,rightTerm);

		//simplify
		terms.remove(leftTerm);
		terms.remove(rightTerm);
		ops.remove(opWithLargestPrecedence);

		//insert newly created expression
		terms.add(indexOfOp,TermNode(expr));
	}

	//now only 2 terms left
	this->term1 = terms.at(0);

	//in case of only one term
	if(ops.size()>0) {
		this->op = ops.at(0);
		this->term2 = Optional.of(terms.at(1));
	}
}
