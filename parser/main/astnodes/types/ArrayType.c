#include <stdio.h>
#include <stdlib.h>

#include "ArrayType.h"
#include "Type.h"

#include "ast/ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct ArrayType* makeArrayType(struct Type* element_type){
	
	struct ArrayType* res = make(ArrayType);

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("ArrayType(...) from: ");
		list_print(tokens);
	}

	struct ArrayType* res = make(ArrayType);

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


