#include <stdio.h>
#include <stdlib.h>

#include "ArrayType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/smalloc.h"

struct ArrayType* makeArrayType(struct Type* element_type){
	
	struct ArrayType* res = smalloc(sizeof(struct ArrayType));

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("ArrayType(...)\n");
	}

	struct ArrayType* res = smalloc(sizeof(struct ArrayType));

	struct TokenList* copy1 = list_copy(tokens);

	if(!list_expect(copy1, LBRACKET)){return NULL;}

	res->element_type = makeType(copy1,debug);
	if(res->element_type == NULL){return NULL;}

	if(!list_expect(copy1, RBRACKET)){return NULL;}

	list_set(tokens, copy1);

	return res;
}

