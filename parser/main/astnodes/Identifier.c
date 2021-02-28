#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Identifier.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct Identifier* makeIdentifier(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Identifier(...) from: ");
		list_print(tokens);
	}

	if(list_size(tokens) == 0){ return NULL; }

	struct Identifier* res = malloc(sizeof(struct Identifier));

	struct Token* tk = list_head(tokens);

	if (tk->kind == ID) {
		strncpy(res->identifier, tk->value_ptr, 19);
		list_consume(tokens, 1);
	} else {
		free(res);
		return NULL;
	}

	return res;
}

