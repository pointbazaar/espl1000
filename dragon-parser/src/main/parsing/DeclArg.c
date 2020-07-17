#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DeclArg.h"
#include "../commandline/TokenList.h"
#include "typenodes/Type.h"
#include "Identifier.h"
#include "../../../../util/util.h"

struct DeclArg* makeDeclArg(struct TokenList* tokens, bool debug) {

	struct DeclArg* res = smalloc(sizeof(struct DeclArg));

	if(debug){
		printf("DeclaredArg(...) from %s",list_code(tokens, debug));
	}

	struct TokenList* copy = list_copy(tokens);

	res->type = makeType2(copy,debug);
	if(res->type == NULL){
		free(res);
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){ 
		free(res);
		return NULL; 
	}
	strcpy(res->name, id->identifier);
	

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeDeclArg(struct DeclArg* da){
	printf("DEBUG: freeDeclArg\n");
	freeType(da->type);
	printf("DEBUG: freeDeclArg 2\n");
	free(da);
}

