#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructMember.h"
#include "Identifier.h"
#include "typenodes/Type.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../token/token.h"
#include "../../../util/util.h"
#include "../../../ast/free_ast.h"
//-------------
struct StructMember* initStructMember();
void beforeAbort(struct StructMember* m, struct TokenList* copy);
//-------------
struct StructMember* makeStructMember(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructMember(...) from: ");
		list_print(tokens);
	}

	struct StructMember* res = initStructMember();

	struct TokenList* copy = list_copy(tokens);

	if((res->type = makeType2(copy, debug)) == NULL){
		
		beforeAbort(res, copy);
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy, debug);
	
	if(id == NULL){
		beforeAbort(res, copy);
		return NULL;
	}
	
	//expect ';'
	struct Token* next = list_head(copy);
	if(next->kind != SEMICOLON){
		beforeAbort(res, copy);
		return NULL;
	}
	list_consume(copy, 1);
	
	strncpy(res->name, id->identifier, DEFAULT_STR_SIZE);
	freeIdentifier(id);

	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}

struct StructMember* initStructMember(){
	
	struct StructMember* res = smalloc(sizeof(struct StructMember));
	res->type = NULL;
	
	return res;
}

void beforeAbort(struct StructMember* m, struct TokenList* copy){
	
	if(m->type != NULL){
		freeType(m->type);
	}
	free(m);
	
	freeTokenListShallow(copy);
}
