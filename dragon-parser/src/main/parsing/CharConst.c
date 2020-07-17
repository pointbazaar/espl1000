#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CharConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../../util/util.h"

struct CharConst* makeCharConst(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("CharConst(...)\n");
	}

	struct CharConst* res = smalloc(sizeof(struct CharConst));

	struct Token* token = list_head(tokens);
	
	if(token == NULL){
		free(res);
		return NULL;
	}

	if (token->kind == CCONST) {
		res->value = token->value[0];
		list_consume(tokens, 1);
	} else {
		//"Error: could not read charConstant node";
		free(res);
		return NULL;
	}

	return res;
}

void freeCharConst(struct CharConst* cc){
	free(cc);
}
