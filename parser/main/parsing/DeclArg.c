#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DeclArg.h"
#include "../commandline/TokenList.h"
#include "typenodes/Type.h"
#include "Identifier.h"
#include "../../../util/util.h"
#include "../../../ast/free_ast.h"

struct DeclArg* makeDeclArg(struct TokenList* tokens, bool debug) {

	struct DeclArg* res = smalloc(sizeof(struct DeclArg));

	if(debug){
		printf("DeclaredArg(...) from ");
		list_print(tokens);
	}

	struct TokenList* copy = list_copy(tokens);

	res->type = makeType2(copy,debug);
	if(res->type == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){ 
		free(res);
		freeType(res->type);
		freeTokenListShallow(copy);
		return NULL; 
	}
	strncpy(res->name, id->identifier, 19);
	freeIdentifier(id);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
