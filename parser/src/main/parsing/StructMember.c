#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructMember.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "typenodes/Type.h"
#include "Identifier.h"
#include "../../../../util/util.h"

struct StructMember* makeStructMember(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructMember(...) from: ");
		list_print(tokens);
	}

	struct StructMember* res = smalloc(sizeof(struct StructMember));

	struct TokenList* copy = list_copy(tokens);

	res->type = makeType2(copy, debug);
	if(res->type == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy, debug);
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

void freeStructMember(struct StructMember* sm){
	freeType(sm->type);
	free(sm);
}
