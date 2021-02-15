#include <stdio.h>
#include <stdlib.h>

#include "ArrayType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

struct ArrayType* makeArrayType(struct Type* element_type){
	
	struct ArrayType* res = malloc(sizeof(struct ArrayType));

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("ArrayType(...) from: ");
		list_print(tokens);
	}

	struct ArrayType* res = malloc(sizeof(struct ArrayType));

	struct TokenList* copy1 = list_copy(tokens);

	if(!list_expect(copy1, LBRACKET)){
		freeTokenListShallow(copy1);
		free(res);
		return NULL;
	}

	res->element_type = makeType2(copy1, debug);
	if(res->element_type == NULL){
		freeTokenListShallow(copy1);
		free(res);
		return NULL;
	}

	if(!list_expect(copy1, RBRACKET)){
		freeTokenListShallow(copy1);
		free(res);
		return NULL;
	}

	list_set(tokens, copy1);
	freeTokenListShallow(copy1);

	return res;
}


