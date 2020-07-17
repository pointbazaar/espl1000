#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BasicTypeWrapped.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "SubrType.h"
#include "SimpleType.h"

struct BasicTypeWrapped* makeBasicTypeWrappedSimple(struct SimpleType* typeNode) {

	struct BasicTypeWrapped* res = malloc(sizeof(struct BasicTypeWrapped));

	res->simpleType = typeNode;
	res->subrType = NULL;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode) {

	struct BasicTypeWrapped* res = malloc(sizeof(struct BasicTypeWrapped));

	res->simpleType = NULL;
	res->subrType = typeNode;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BasicTypeWrapped(...) from: %s\n", list_code(tokens, debug));
	}

	struct BasicTypeWrapped* res = malloc(sizeof(struct BasicTypeWrapped));
	if(res == NULL){return NULL;}
	
	res->simpleType = NULL;
	res->subrType = NULL;

	struct TokenList* copy = list_copy(tokens);

	if (list_size(copy) > 1 && tokenEquals(list_head(copy), makeToken(LPARENS)) ) {
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, LPARENS)){return NULL;}

		res->subrType = makeSubrType(copy2,debug);
		if(res->subrType == NULL){return NULL;}

		if(!list_expect(copy2, RPARENS)){return NULL;}

		list_set(copy, copy2);

	} else {
		res->simpleType = makeSimpleType2(copy,debug);
		if(res->simpleType == NULL){return NULL;}
	}

	list_set(tokens, copy);

	if(debug){
		printf("\tsuccess parsing BasicTypeWrapped\n");
	}

	return res;
}

void freeBasicTypeWrapped(struct BasicTypeWrapped* btw){
	
	printf("DEBUG: freeBasicTypeWrapped\n");
	if(btw->simpleType != NULL){
		freeSimpleType(btw->simpleType);
		
	}else if(btw->subrType != NULL){
		freeSubrType(btw->subrType);
	}
	printf("DEBUG: freeBasicTypeWrapped 2\n");
	
	//TODO
	//free(btw);
}
