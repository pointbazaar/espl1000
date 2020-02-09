#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <algorithm>

//project headers
#include "ExpressionNode.hpp"
#include "../commandline/TokenList.hpp"
#include "TermNode.hpp"
#include "OperatorNode.hpp"

ExpressionNode::ExpressionNode(TermNode* term) {
	this->term1 = term;
}

ExpressionNode::ExpressionNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "ExpressionNode(...)" << endl;
	}

	// temporary containers
	vector<OperatorNode*> operatorNodes;
	vector<TermNode*> termNodes;
	// end of temporary containers
	TokenList copy = tokens.copy();
	termNodes.push_back(new TermNode(copy,debug));
	try {

		while (true) {
			TokenList copy2 = TokenList(copy);

			OperatorNode* myop = new OperatorNode(copy2,debug);
			TermNode* myterm = new TermNode(copy2,debug);

			operatorNodes.push_back(myop);
			termNodes.push_back(myterm);

			copy.set(copy2);
		}
	} catch (string e) {
		//pass
	}

	tokens.set(copy);

	performTreeTransformation(operatorNodes,termNodes);
}

ExpressionNode::ExpressionNode(TermNode* leftTerm, OperatorNode* op, TermNode* rightTerm) {
	this->term1=leftTerm;
	this->op=optional<OperatorNode*>(op);
	this->term2=optional<TermNode*>(rightTerm);
}

void ExpressionNode::performTreeTransformation(
		vector<OperatorNode*> ops,
		vector<TermNode*> terms
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
		OperatorNode* opWithLargestPrecedence = ops.at(0);
		vector<string>::iterator lowest = operatorPrecedence.end();

		for(OperatorNode* o1 : ops){
			if(find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op) < lowest){
				lowest = find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op);
				opWithLargestPrecedence = o1;
			}
		}
		vector<OperatorNode*>::iterator indexOfOp = find(ops.begin(),ops.end(),opWithLargestPrecedence);


		TermNode* leftTerm = terms.at(indexOfOp - ops.begin());
		TermNode* rightTerm = terms.at((indexOfOp - ops.begin())+1);

		ExpressionNode expr = ExpressionNode(leftTerm,opWithLargestPrecedence,rightTerm);

		//simplify
		vector<TermNode*>::iterator i1;
		i1 = find(terms.begin(),terms.end(),leftTerm);
		terms.erase(i1);
		i1 = find(terms.begin(),terms.end(),rightTerm);
		terms.erase(i1);

		vector<OperatorNode*>::iterator i2 = find(ops.begin(),ops.end(),opWithLargestPrecedence);
		ops.erase(i2);

		//insert newly created expression
		TermNode* ttmp = new TermNode(&expr);
		vector<TermNode*>::iterator indexOfOpIt = terms.begin() + (indexOfOp - ops.begin());
		terms.insert(indexOfOpIt,ttmp);
	}

	//now only 2 terms left
	this->term1 = terms.at(0);

	//in case of only one term
	if(ops.size()>0) {
		this->op = ops.at(0);
		this->term2 = optional<TermNode*>(terms.at(1));
	}
}
