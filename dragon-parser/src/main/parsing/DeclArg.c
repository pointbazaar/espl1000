#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DeclArg.h"
#include "../commandline/TokenList.h"
#include "typenodes/Type.h"
#include "Identifier.h"

struct DeclArg* makeDeclArg(struct TokenList* tokens, bool debug) {

	struct DeclArg* res = malloc(sizeof(struct DeclArg));

	if(debug){
		printf("DeclaredArg(...) from %s",list_code(tokens, debug));
	}

	struct TokenList* copy = list_copy(tokens);

	res->type = makeType2(copy,debug);
	if(res->type == NULL){return NULL;}

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){ return NULL; }
	res->name[0]='\0';
	strcpy(res->name, id->identifier);
	

	list_set(tokens, copy);

	return res;
}

