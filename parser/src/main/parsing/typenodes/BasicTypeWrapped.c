#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BasicTypeWrapped.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "SubrType.h"
#include "SimpleType.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct BasicTypeWrapped* makeBasicTypeWrappedSimple(struct SimpleType* typeNode) {

	struct BasicTypeWrapped* res = smalloc(sizeof(struct BasicTypeWrapped));

	res->simpleType = typeNode;
	res->subrType = NULL;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode) {

	struct BasicTypeWrapped* res = smalloc(sizeof(struct BasicTypeWrapped));

	res->simpleType = NULL;
	res->subrType = typeNode;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BasicTypeWrapped(...) from: ");
		list_print(tokens);
	}

	struct BasicTypeWrapped* res = smalloc(sizeof(struct BasicTypeWrapped));
	
	res->simpleType = NULL;
	res->subrType = NULL;

	struct TokenList* copy = list_copy(tokens);
	struct Token* lparens = makeToken(LPARENS);

	if (
		list_size(copy) > 1 
		&& list_startsWith(copy, lparens)
	){
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, LPARENS)){
			freeTokenListShallow(copy2);
			free(res);
			freeTokenListShallow(copy);
			freeToken(lparens);
			return NULL;
		}

		res->subrType = makeSubrType(copy2,debug);
		if(res->subrType == NULL){
			freeTokenListShallow(copy2);
			free(res);
			freeTokenListShallow(copy);
			freeToken(lparens);
			return NULL;
		}

		if(!list_expect(copy2, RPARENS)){
			freeTokenListShallow(copy2);
			free(res);
			freeTokenListShallow(copy);
			freeToken(lparens);
			return NULL;
		}

		list_set(copy, copy2);
		freeTokenListShallow(copy2);

	} else {
		res->simpleType = makeSimpleType2(copy,debug);
		if(res->simpleType == NULL){
			free(res);
			freeTokenListShallow(copy);
			freeToken(lparens);
			return NULL;
		}
	}
	
	freeToken(lparens);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	if(debug){
		printf("\tsuccess parsing BasicTypeWrapped\n");
	}

	return res;
}


