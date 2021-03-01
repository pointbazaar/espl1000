#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "TypeParam.h"

#include "ast/ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct TypeParam* makeTypeParam(struct TokenList* tokens, bool debug){

	if(debug){
		printf("TypeParam(...)\n");
	}

	struct Token* token = list_head(tokens);
	if(token == NULL){
		return NULL;
	}
	
	struct TypeParam* res = malloc(sizeof(struct TypeParam));

	if (token->kind == TPARAM) {
		res->index = atoi(token->value_ptr);
		if(list_size(tokens) == 0){
			free(res);
			return NULL;
		}
		list_consume(tokens, 1);
	} else {
		//("Error: could not read type parameter node");
		free(res);
		return NULL;
	}

	return res;
}

