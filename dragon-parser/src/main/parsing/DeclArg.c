#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DeclArg.h"
#include "../commandline/TokenList.h"
#include "../commandline/smalloc.h"
#include "typenodes/Type.h"
#include "Identifier.h"

struct DeclArg* makeDeclArg(struct TokenList* tokens, bool debug) {

	struct DeclArg* res = smalloc(sizeof(struct DeclArg));

	if(debug){
		printf("DeclaredArg(...) from %s",list_code(tokens));
	}

	struct TokenList* copy = list_copy(tokens);

	res->type = makeType2(copy,debug);
	if(res->type == NULL){return NULL;}

	res->name = makeIdentifier(copy,debug)->identifier;
	if(res->name == NULL){return NULL;}

	list_set(tokens, copy);

	return res;
}

