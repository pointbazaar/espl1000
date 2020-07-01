#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Identifier.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"

struct Identifier* makeIdentifier(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Identifier(...) from: %s\n", list_code(tokens));
	}

	struct Identifier* res = malloc(sizeof(struct Identifier));
	if(res == NULL){return NULL;}

	if(list_size(tokens) == 0){
		return NULL;
	}
	struct Token* token = list_head(tokens);

	if (token->kind == ID) {
		res->identifier = token->value;
		list_consume(tokens, 1);

	} else {
		char msg[200];
		sprintf(msg,"Error: could not read identifier from token: ");
		strcat(msg,token->value);
		strcat(msg," in ");
		strcat(msg,tokens->relPath);
		strcat(msg,":");

		char my[10];
		my[0]='\0';
		sprintf(my,"%d",token->lineNumber);

		strcat(msg,my);
		strcat(msg," in context '"); 
		strcat(msg,list_code(tokens));
		strcat(msg,"'");

		return NULL;
	}

	return res;
}

