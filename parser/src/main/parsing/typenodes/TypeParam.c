#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "TypeParam.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../../util/util.h"

struct TypeParam* makeTypeParam(struct TokenList* tokens, bool debug){

	if(debug){
		printf("TypeParam(...)\n");
	}

	struct Token* token = list_head(tokens);
	if(token == NULL){
		return NULL;
	}
	
	struct TypeParam* res = smalloc(sizeof(struct TypeParam));

	if (token->kind == TPARAM) {
		res->index = atoi(token->value);
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

void freeTypeParam(struct TypeParam* tp){
	
	free(tp);
}
