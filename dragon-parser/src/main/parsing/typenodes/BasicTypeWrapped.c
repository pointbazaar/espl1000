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

	res->m1 = typeNode;
	res->m2 = NULL;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode) {

	struct BasicTypeWrapped* res = malloc(sizeof(struct BasicTypeWrapped));

	res->m1 = NULL;
	res->m2 = typeNode;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BasicTypeWrapped(...) from: %s\n", list_code(tokens, debug));
	}

	struct BasicTypeWrapped* res = malloc(sizeof(struct BasicTypeWrapped));
	if(res == NULL){return NULL;}

	struct TokenList* copy = list_copy(tokens);

	if (list_size(copy) > 1 && tokenEquals(list_head(copy), makeToken(LPARENS)) ) {
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, LPARENS)){return NULL;}

		res->m2 = makeSubrType(copy2,debug);
		if(res->m2 == NULL){return NULL;}

		if(!list_expect(copy2, RPARENS)){return NULL;}

		list_set(copy, copy2);

	} else {
		res->m1 = makeSimpleType2(copy,debug);
		if(res->m1 == NULL){return NULL;}
	}

	list_set(tokens, copy);

	if(debug){
		printf("\tsuccess parsing BasicTypeWrapped\n");
	}

	return res;
}

