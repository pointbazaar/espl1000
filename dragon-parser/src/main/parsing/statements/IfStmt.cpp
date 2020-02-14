
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <iostream>

#include "IfStmt.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../Expr.hpp"
#include "Stmt.hpp"

using namespace std;

struct IfStmt* makeIfStmt(TokenList* tokens, bool debug) {

	if(debug){
		cout << "IfStmt(...)" << endl; 
	}

	struct IfStmt* res = (struct IfStmt*)malloc(sizeof(struct IfStmt));
	res->condition = NULL;
	vector<Stmt*> r;
	vector<Stmt*> r2;
	res->statements = &r;
	res->elseStatements = &r2;

	TokenList copy = TokenList(*tokens);

	if(copy.size() < 3){
		throw string("not enough tokens");
	}

	copy.expect(IF);
	res->condition = makeExpr(&copy,debug);
	copy.expect(LCURLY);

	Token next = copy.get(0);
	
	while (!(next.kind == RCURLY)) {

		res->statements->push_back(new Stmt(copy,debug));
		next = copy.get(0);
	}

	copy.expect(RCURLY);

	//maybe there is an else
	if (copy.startsWith(ELSE)) {

		copy.expect(ELSE);

		copy.expect(LCURLY);

		//maybe there be some statements
		Token elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			res->elseStatements->push_back(new Stmt(copy,debug));
			elsenext = copy.get(0);
		}

		copy.expect(RCURLY);
	}

	tokens->set(copy);

	return res;
}

