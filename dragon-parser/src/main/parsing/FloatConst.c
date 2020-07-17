#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "FloatConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../../../../util/util.h"

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug){

	if(debug){
		printf("FloatConst(...)\n");
	}

	if(list_size(tokens) == 0){
		return NULL;
	}

	struct FloatConst* res = smalloc(sizeof(struct FloatConst));

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);

	float f = 1.0;
	if(tk->kind == OPKEY && strcmp(tk->value, "-") == 0){
		f = -1.0;
		list_consume(copy, 1);

		if(debug){
			printf("parsed sign\n");
		}
	}

	if(debug){
		printf("%s\n", list_code(copy,debug));
	}

	if(list_size(copy) == 0){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(list_head(copy)->kind == FLOATING){

		res->value = atof(list_head(copy)->value);
		list_consume(copy, 1);

	}else{
		//could not find a float const
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	res->value *= f;

	//--------------------------

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeFloatConst(struct FloatConst* fc){
	free(fc);
}
