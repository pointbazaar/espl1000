#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoolConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("makeBoolConst(...) from: ");
		list_print(tokens);
	}

	if(list_size(tokens) == 0){
		return NULL;
	}
	
	struct BoolConst* res = make(BoolConst);
	
	struct Token* tk = list_head(tokens);
	
	if (tk->kind == BCONST_TRUE) {
		
		res->value = true;
		list_consume(tokens, 1);
		
	}else if (tk->kind == BCONST_FALSE) {
		
		res->value = false;
		list_consume(tokens, 1);
		
	} else {
		if(debug){
			printf("token was not the right kind.\n");
		}
		free(res);
		return NULL;
	}

	return res;
}
