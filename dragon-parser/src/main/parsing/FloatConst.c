#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "FloatConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"

#include <stdbool.h>

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug){

	struct FloatConst* res = malloc(sizeof(struct FloatConst));

	if(debug){
		printf("FloatConst(...)\n");
	}
	if(list_size(tokens) == 0){return NULL;}

	struct TokenList* copy = list_copy(tokens);

	//---------------------------

	struct Token* tk = list_head(copy);

	float f = 1.0;
	if(tk->kind == OPKEY && strcmp(tk->value, "-") == 0){
		f = -1.0;
		list_consume(copy, 1);
	}

	if(list_size(copy) == 0){return NULL;}

	if(list_get(copy, 0)->kind == FLOATING){

		res->value = atof(list_get(copy, 0)->value);
		list_consume(copy, 1);

	}else{
		//could not find a float const
		return NULL;
	}

	res->value *= f;

	//--------------------------

	list_set(tokens, copy);

	return res;
}