
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

	struct IfStmt* res = (struct IfStmt*)malloc(sizeof(struct IfStmt));
	res->condition = NULL;
	vector<Stmt*> r;
	vector<Stmt*> r2;
	res->statements = &r;
	res->elseStatements = &r2;

	if(debug){
		cout << "IfStmt(...)" << endl; 
	}

	TokenList copy = TokenList(*tokens);

	if(copy.size() < 3){
		throw string("not enough tokens");
	}

	copy.expect(Token(IF));
	res->condition = makeExpr(&copy,debug);
	copy.expect(Token(LCURLY));

	Token next = copy.get(0);
	
	while (!(next.kind == RCURLY)) {

		res->statements->push_back(new Stmt(copy,debug));
		next = copy.get(0);
	}

	copy.expect(Token(RCURLY));

	//maybe there is an else
	if (copy.startsWith(Token(ELSE))) {

		copy.expect(Token(ELSE));

		copy.expect(Token(LCURLY));

		//maybe there be some statements
		Token elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			res->elseStatements->push_back(new Stmt(copy,debug));
			elsenext = copy.get(0);
		}

		copy.expect(Token(RCURLY));
	}

	tokens->set(copy);

	return res;
}

