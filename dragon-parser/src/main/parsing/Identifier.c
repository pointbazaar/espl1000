#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Identifier.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"

struct Identifier* makeIdentifier(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Identifier(...) from: %s\n", list_code(tokens, debug));
	}

	struct Identifier* res = malloc(sizeof(struct Identifier));
	if(res == NULL){return NULL;}

	if(list_size(tokens) == 0){ return NULL; }
	struct Token* tk = list_head(tokens);

	if (tk->kind == ID) {
		res->identifier = tk->value;
		list_consume(tokens, 1);
	} else {
		return NULL;
	}

	return res;
}

void freeIdentifier(struct Identifier* id){
	//TODO:
	//think about id->identifier
	free(id);
}
