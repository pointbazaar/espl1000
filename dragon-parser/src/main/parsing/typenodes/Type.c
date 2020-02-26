#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Type.h"
#include "../../commandline/TokenList.h"
#include "ArrayType.h"
#include "TypeParam.h"
#include "BasicTypeWrapped.h"
#include "../../commandline/smalloc.h"

struct Type* makeType(struct BasicTypeWrapped* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m1 = typeNode;

	return res;
}

struct Type* makeType(struct ArrayType* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m3 = typeNode;

	return res;
}

struct Type* makeType(struct TypeParam* typeNode){

	struct Type* res = smalloc(sizeof(struct Type));

	res->m2 = typeNode;

	return res;
}

struct Type* makeType(struct TokenList* tokens, bool debug){

	if(debug){
		printf("Type(...) from: %s\n",list_code(tokens));
	}

	struct Type* res = smalloc(sizeof(struct Type));

	struct TokenList* copy = list_copy(tokens);

	res->m3 	= makeArrayType			(copy,debug);
	if(res->m3 == NULL){
		res->m2 = makeTypeParam			(copy,debug);
		if(res->m2 == NULL) {
			res->m1 = makeBasicTypeWrapped	(copy,debug);
			if(res->m1 == NULL){
				return NULL;
			}
		}
	}

	list_set(tokens, copy);

	return res;
}

