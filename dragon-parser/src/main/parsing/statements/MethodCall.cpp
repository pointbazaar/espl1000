#include <vector>
#include <iostream>

#include "MethodCall.hpp"
#include "../Identifier.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"

#include <stdio.h>

struct MethodCall* makeMethodCall(TokenList tokens,bool debug) {

	if(debug){
		cout << "MethodCall(...)" << endl;
	}

	struct MethodCall* res = (struct MethodCall*)malloc(sizeof(struct MethodCall));

	if(res==NULL){
		printf("could not malloc\n");
		exit(1);
	}

	res->args = (struct Expr**)malloc(sizeof(struct Expr*)*1);

	res->count_args = 0;

	TokenList copy = tokens.copy();

	res->methodName = (char*)Identifier(&copy,debug).identifier.c_str();

	copy.expect(LPARENS);

	Token next = copy.get(0);
	bool found = false;
	while (next.kind != RPARENS) {
		if (found) {
			copy.expect(COMMA);
		}

		res->args[res->count_args++] = makeExpr(&copy,debug);
		res->args = (struct Expr**)realloc(res->args, sizeof(struct Expr*) * res->count_args);

		next = copy.get(0);
		found = true;
	}

	copy.expect(RPARENS);

	tokens.set(copy);

	return res;
}

