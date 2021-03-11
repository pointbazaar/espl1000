#include <stdio.h>
#include <stdlib.h>

#include "MethodCall.h"
#include "../Identifier.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug) {

	if(debug){
		printf("MethodCall(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL;}

	struct MethodCall* res = make(MethodCall);

	res->args = malloc(sizeof(struct Expr*)*1);
	res->count_args = 0;

	struct TokenList* copy = list_copy(tokens);

	struct Identifier* id = makeIdentifier(copy,debug);
	
	if(id == NULL){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	strcpy(res->methodName, id->identifier);
	freeIdentifier(id);

	if(list_size(copy) == 0){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(!list_expect(copy, LPARENS)){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(debug){
		printf("try to parse args\n");
	}

	if(list_size(copy) == 0){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	struct Token* next = list_head(copy);

	bool found = false;
	while (next->kind != RPARENS) {
		if (found) {
			if(!list_expect(copy, COMMA)){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
		}

		struct Expr* expr = makeExpr(copy,debug);
		if(expr == NULL){
			free(res->args);
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}

		res->args[res->count_args] = expr;
		res->count_args += 1;

		res->args = realloc(res->args, sizeof(struct Expr*) * (res->count_args + 1));

		next = list_head(copy);
		if(next == NULL){
			free(res->args);
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}

		found = true;
	}

	if(!list_expect(copy, RPARENS)){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing MethodCall\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


