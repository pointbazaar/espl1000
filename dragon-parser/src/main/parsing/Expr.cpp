#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <algorithm>

#include "Expr.hpp"
#include "../commandline/TokenList.hpp"
#include "Term.hpp"
#include "Op.hpp"

Expr::Expr(Term* term) {
	this->term1 = term;
}

Expr::Expr(TokenList tokens, bool debug) {

	if(debug){
		cout << "Expr(...)" << endl;
	}

	// temporary containers
	vector<Op*> operatorNodes;
	vector<Term*> termNodes;
	// end of temporary containers
	TokenList copy = tokens.copy();
	termNodes.push_back(new Term(copy,debug));
	try {

		while (true) {
			TokenList copy2 = TokenList(copy);

			Op* myop = new Op(copy2,debug);
			Term* myterm = new Term(copy2,debug);

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

Expr::Expr(Term* leftTerm, Op* op, Term* rightTerm) {
	this->term1=leftTerm;
	this->op=optional<Op*>(op);
	this->term2=optional<Term*>(rightTerm);
}

void Expr::performTreeTransformation(
		vector<Op*> ops,
		vector<Term*> terms
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
		Op* opWithLargestPrecedence = ops.at(0);
		vector<string>::iterator lowest = operatorPrecedence.end();

		for(Op* o1 : ops){
			if(find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op) < lowest){
				lowest = find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op);
				opWithLargestPrecedence = o1;
			}
		}
		vector<Op*>::iterator indexOfOp = find(ops.begin(),ops.end(),opWithLargestPrecedence);


		Term* leftTerm = terms.at(indexOfOp - ops.begin());
		Term* rightTerm = terms.at((indexOfOp - ops.begin())+1);

		Expr expr = Expr(leftTerm,opWithLargestPrecedence,rightTerm);

		//simplify
		vector<Term*>::iterator i1;
		i1 = find(terms.begin(),terms.end(),leftTerm);
		terms.erase(i1);
		i1 = find(terms.begin(),terms.end(),rightTerm);
		terms.erase(i1);

		vector<Op*>::iterator i2 = find(ops.begin(),ops.end(),opWithLargestPrecedence);
		ops.erase(i2);

		//insert newly created expression
		Term* ttmp = new Term(&expr);
		vector<Term*>::iterator indexOfOpIt = terms.begin() + (indexOfOp - ops.begin());
		terms.insert(indexOfOpIt,ttmp);
	}

	//now only 2 terms left
	this->term1 = terms.at(0);

	//in case of only one term
	if(ops.size()>0) {
		this->op = ops.at(0);
		this->term2 = optional<Term*>(terms.at(1));
	}
}
