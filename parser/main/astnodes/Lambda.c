#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "expr/Expr.h"
#include "Identifier.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

static void freeIdentifiers(struct Lambda* l);

struct Lambda* makeLambda(struct TokenList* tokens, bool debug) {
	
	struct TokenList* copy = list_copy(tokens);
	
	struct Lambda* res = make(Lambda);
	
	res->count_params = 0;
	
	if(!list_expect(copy, LPARENS)){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	res->params[0] = makeIdentifier(copy, debug);
	res->count_params += 1;
	
	if(res->params[0] == NULL){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	while(list_head(copy)->kind == COMMA){
		
		if(res->count_params >= LAMBDA_MAX_PARAMS){
			printf("too many lambda parameters.\n");
			printf("exiting.\n");
			exit(1);
		}
		
		if(!list_expect(copy, COMMA)){
			freeIdentifiers(res);
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}
		
		res->params[res->count_params] = makeIdentifier(copy, debug);
		res->count_params += 1;
	}
	
	if(!list_expect(copy, RPARENS)){
		freeIdentifiers(res);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(!list_expect(copy, ARROW)){
		freeIdentifiers(res);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	res->expr = makeExpr(copy, debug);
	
	if(res->expr == NULL){
		freeIdentifiers(res);
		free(res);
		
		freeTokenListShallow(copy);
		return NULL;
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}

static void freeIdentifiers(struct Lambda* l){
	
	for(uint8_t i = 0; i < l->count_params; i++){
		freeIdentifier(l->params[i]);
	}
}
