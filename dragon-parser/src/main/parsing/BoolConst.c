#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoolConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug) {

	struct BoolConst* res = malloc(sizeof(struct BoolConst));

	if(debug){
		printf("BoolConst(...)\n");
	}

	struct TokenList* copy = makeTokenList(tokens);

	if(list_size(copy) == 0){return NULL;}
	
	if (list_get(copy, 0)->kind == BCONST) {
		struct Token* tk = list_get(copy, 0);

		if( strcmp(tk->value,"") != 0 && strcmp(tk->value,"true") == 0) {
			res->value = true;
		}else {
			res->value = false;
		}
		if(!list_consume(copy, 1)){return NULL;}
	} else {
		//"could not read Bool Constant node";
		return NULL;
	}

	list_set(tokens, copy);

	return res;
}