#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "FloatConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug){

	struct FloatConst* res = malloc(sizeof(struct FloatConst));

	if(debug){
		printf("FloatConst(...)\n");
	}
	if(list_size(tokens) == 0){return NULL;}

	if(list_get(tokens, 0)->kind == FLOATING){
		res->value = atof(list_get(tokens, 0)->value);
	}else{
		//"could not find a float const";
		return NULL;
	}

	return res;
}