#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoolConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug) {

	struct BoolConst* res = malloc(sizeof(struct BoolConst));

	if(debug){
		printf("makeBoolConst(...) from: %s\n", list_code(tokens, debug));
	}

	struct TokenList* copy = list_copy(tokens);

	if(list_size(copy) == 0){
		return NULL;
	}
	
	if (list_get(copy, 0)->kind == BCONST) {
		struct Token* tk = list_get(copy, 0);

		if(strcmp(tk->value,"true") == 0) {
			res->value = true;
		}else if(strcmp(tk->value,"false") == 0){
			res->value = false;
		}else{
			if(debug){
				printf("tk->value was not satisfactory.\n");
			}
			return NULL;
		}
		list_consume(copy, 1);
	} else {
		if(debug){
			printf("token was not the right kind.\n");
		}
		return NULL;
	}

	list_set(tokens, copy);

	return res;
}