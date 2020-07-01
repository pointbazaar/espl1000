#include <stdio.h>
#include <stdlib.h>

#include "MethodCall.h"
#include "../Identifier.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../Expr.h"


struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug) {

	if(debug){
		printf("MethodCall(...)\n");
	}

	struct MethodCall* res = malloc(sizeof(struct MethodCall));
	if(res==NULL){return NULL;}

	res->args = malloc(sizeof(struct Expr*)*1);

	res->count_args = 0;

	struct TokenList* copy = list_copy(tokens);

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){return NULL;}

	res->methodName = id->identifier;

	if(!list_expect(copy, LPARENS)){return NULL;}

	if(list_size(copy) == 0){return NULL;}
	struct Token* next = list_head(copy);

	bool found = false;
	while (next->kind != RPARENS) {
		if (found) {
			if(!list_expect(copy, COMMA)){return NULL;}
		}

		struct Expr* expr = makeExpr(copy,debug);
		if(expr == NULL){return NULL;}

		res->args[res->count_args++] = expr;
		res->args = realloc(res->args, sizeof(struct Expr*) * res->count_args);

		next = list_get(copy, 0);
		if(next == NULL){return NULL;}

		found = true;
	}

	if(!list_expect(copy, RPARENS)){return NULL;}

	list_set(tokens, copy);

	return res;
}

