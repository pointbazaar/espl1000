#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BasicTypeWrapped.h"
#include "SubrType.h"
#include "SimpleType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct BasicTypeWrapped* makeBasicTypeWrappedSimple(struct SimpleType* typeNode) {

	struct BasicTypeWrapped* res = make(BasicTypeWrapped);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->simpleType = typeNode;
	res->subrType = NULL;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode) {

	struct BasicTypeWrapped* res = make(BasicTypeWrapped);

	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;
	
	res->simpleType = NULL;
	res->subrType = typeNode;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BasicTypeWrapped(...) from: ");
		list_print(tokens);
	}

	struct BasicTypeWrapped* res = make(BasicTypeWrapped);
	
	res->super.line_num    = list_head(tokens)->line_num;
	res->super.annotations = 0;
	
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
		res->simpleType = makeSimpleType(copy,debug);
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


