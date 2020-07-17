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
		printf("makeStructMember(...) from: %s\n", list_code(tokens, debug));
	}

	struct StructMember* res = smalloc(sizeof(struct StructMember));

	struct TokenList* copy = list_copy(tokens);

	struct Type* type = makeType2(copy, debug);
	if(type == NULL){
		free(res);
		return NULL;
	}
	res->type = type;

	struct Identifier* id = makeIdentifier(copy, debug);
	if(id == NULL){
		free(res);
		return NULL;
	}
	res->name = id->identifier;

	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}

void freeStructMember(struct StructMember* sm){
	//TODO: inline sm->name into the struct
	freeType(sm->type);
	free(sm);
}
