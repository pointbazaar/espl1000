#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Type.h"
#include "../../commandline/TokenList.h"
#include "ArrayType.h"
#include "TypeParam.h"
#include "BasicTypeWrapped.h"
#include "../../../../../util/util.h"

struct Type* makeType_1(struct BasicTypeWrapped* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m1 = typeNode;
	res->m2 = NULL;
	res->m3 = NULL;

	return res;
}

struct Type* makeType_2(struct TypeParam* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m1 = NULL;
	res->m2 = typeNode;
	res->m3 = NULL;

	return res;
}

struct Type* makeType_3(struct ArrayType* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = typeNode;

	return res;
}

struct Type* makeType2(struct TokenList* tokens, bool debug){

	if(debug){
		printf("Type(...) from: ");
		list_print(tokens);
	}

	struct Type* res = smalloc(sizeof(struct Type));
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;

	struct TokenList* copy = list_copy(tokens);

	res->m3 	= makeArrayType2			(copy,debug);
	if(res->m3 == NULL){
		res->m2 = makeTypeParam			(copy,debug);
		if(res->m2 == NULL) {
			res->m1 = makeBasicTypeWrapped2	(copy,debug);
			if(res->m1 == NULL){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
		}
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	if(debug){
		printf("\tsuccess parsing Type\n");
	}

	return res;
}

void freeType(struct Type* t){
	
	printf("DEBUG: freeType\n");
	
	if(t->m1 != NULL){
		freeBasicTypeWrapped(t->m1);
	}else if(t->m2 != NULL){
		freeTypeParam(t->m2);
	}else if(t->m3 != NULL){
		freeArrayType(t->m3);
	}
	printf("DEBUG: freeType 2\n");
	free(t);
}
