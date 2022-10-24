#include <stdio.h>
#include <stdlib.h>
#include <var/Variable.h>

#include "parser/main/util/parse_astnode.h"

#include "Call.h"
#include "../Identifier.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct Call* makeCall(struct TokenList* tokens) {
	
	if(list_size(tokens) < 3){ return NULL;}

	struct Call* res = make(Call);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));

	res->args = malloc(sizeof(struct Expr*)*1);
	res->count_args = 0;

	res->callable = makeVariable(copy);

	if(res->callable == NULL){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(list_size(copy) == 0){
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(!list_expect(copy, LPARENS)){
		free_variable(res->callable);
		free(res->args);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(list_size(copy) == 0){
		free_variable(res->callable);
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

		struct Expr* expr = makeExpr(copy);
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

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


