#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoolConst.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("makeBoolConst(...) from: ");
		list_print(tokens);
	}

	if(list_size(tokens) == 0){
		return NULL;
	}
	
	struct BoolConst* res = malloc(sizeof(struct BoolConst));
	
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