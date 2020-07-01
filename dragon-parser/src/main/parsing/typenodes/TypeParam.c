#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "TypeParam.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"

struct TypeParam* makeTypeParam(struct TokenList* tokens, bool debug){

	if(debug){
		printf("TypeParam(...)\n");
	}

	struct TypeParam* res = malloc(sizeof(struct TypeParam));

	struct Token* token = list_get(tokens, 0);
	if(token == NULL){return NULL;}

	if (token->kind == TPARAM) {
		res->typeParameterIndex = atoi(token->value);
		if(list_size(tokens) == 0){
			return NULL;
		}
		list_consume(tokens, 1);
	} else {
		//("Error: could not read type parameter node");
		return NULL;
	}

	return res;
}

