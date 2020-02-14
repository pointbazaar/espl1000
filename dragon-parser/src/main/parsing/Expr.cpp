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

struct Expr* makeExpr(struct Term* term) {
	struct Expr* res = (struct Expr*)malloc(sizeof(struct Expr));
	res->term1 = term;
	res->op    = NULL;
	res->term2 = NULL;
	return res;
}

struct Expr* makeExpr(TokenList* tokens, bool debug) {

	if(debug){
		cout << "Expr(...) " << endl;
		cout << "from " << tokens->code() << endl;
	}

	// temporary containers
	vector<struct Op*> operatorNodes;
	vector<struct Term*> termNodes;
	// end of temporary containers
	TokenList copy = tokens->copy();
	termNodes.push_back(makeTerm(&copy,debug));
	try {

		while (copy.size()>=2) {
			TokenList copy2 = TokenList(copy);

			struct Op* myop = makeOp(&copy2,debug);
			struct Term* myterm = makeTerm(&copy2,debug);

			operatorNodes.push_back(myop);
			termNodes.push_back(myterm);

			copy.set(copy2);
		}
	} catch (string e) {
		//pass
	}

	tokens->set(copy);

	return performTreeTransformation(operatorNodes,termNodes);
}

struct Expr* makeExpr(struct Term* leftTerm, struct Op* op, struct Term* rightTerm) {

	struct Expr* res = (struct Expr*)malloc(sizeof(struct Expr));
	res->term1 = leftTerm;
	res->op    = op;
	res->term2 = rightTerm;
	return res;
}

struct Expr* performTreeTransformation(
		vector<struct Op*> ops,
		vector<struct Term*> terms
){
	//transform the list into a tree, respecting operator precedence

	struct Expr* res = (struct Expr*)malloc(sizeof(struct Expr));
	res->term1 = NULL;
	res->op    = NULL;
	res->term2 = NULL;
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
		struct Op* opWithLargestPrecedence = ops.at(0);
		vector<string>::iterator lowest = operatorPrecedence.end();

		for(struct Op* o1 : ops){
			if(find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op) < lowest){
				lowest = find(operatorPrecedence.begin(),operatorPrecedence.end(),o1->op);
				opWithLargestPrecedence = o1;
			}
		}
		vector<struct Op*>::iterator indexOfOp = find(ops.begin(),ops.end(),opWithLargestPrecedence);


		struct Term* leftTerm = terms.at(indexOfOp - ops.begin());
		struct Term* rightTerm = terms.at((indexOfOp - ops.begin())+1);

		struct Expr* expr = makeExpr(leftTerm,opWithLargestPrecedence,rightTerm);

		//simplify
		vector<struct Term*>::iterator i1;
		i1 = find(terms.begin(),terms.end(),leftTerm);
		terms.erase(i1);
		i1 = find(terms.begin(),terms.end(),rightTerm);
		terms.erase(i1);

		vector<struct Op*>::iterator i2 = find(ops.begin(),ops.end(),opWithLargestPrecedence);
		ops.erase(i2);

		//insert newly created expression
		struct Term* ttmp = makeTerm(expr);
		vector<struct Term*>::iterator indexOfOpIt = terms.begin() + (indexOfOp - ops.begin());
		terms.insert(indexOfOpIt,ttmp);
	}

	//now only 2 terms left
	res->term1 = terms.at(0);

	//in case of only one term
	if(ops.size()>0) {
		res->op = ops.at(0);
		res->term2 = terms.at(1);
	}

	return res;
}
