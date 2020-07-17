#include <stdio.h>
#include <stdlib.h>

#include "ArrayType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"

struct ArrayType* makeArrayType(struct Type* element_type){
	
	struct ArrayType* res = malloc(sizeof(struct ArrayType));

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("ArrayType(...) from: %s\n", list_code(tokens, debug));
	}

	struct ArrayType* res = malloc(sizeof(struct ArrayType));
	if(res == NULL){return NULL;}

	struct TokenList* copy1 = list_copy(tokens);

	if(!list_expect(copy1, LBRACKET)){return NULL;}

	res->element_type = makeType2(copy1, debug);
	if(res->element_type == NULL){return NULL;}

	if(!list_expect(copy1, RBRACKET)){return NULL;}

	list_set(tokens, copy1);
	freeTokenListShallow(copy1);

	return res;
}

void freeArrayType(struct ArrayType* at){
	freeType(at->element_type);
	free(at);
}
