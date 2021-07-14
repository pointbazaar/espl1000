#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "BasicType.h"
#include "SubrType.h"
#include "SimpleType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct BasicType* makeBasicTypeSimple(struct SimpleType* typeNode) {

	struct BasicType* res = make(BasicType);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->simple_type = typeNode;
	res->subr_type = NULL;

	return res;
}

struct BasicType* makeBasicTypeSubr(struct SubrType* typeNode) {

	struct BasicType* res = make(BasicType);

	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;
	
	res->simple_type = NULL;
	res->subr_type   = typeNode;

	return res;
}

struct BasicType* makeBasicType2(struct TokenList* tokens) {

	struct BasicType* res = make(BasicType);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->simple_type = NULL;
	res->subr_type   = NULL;
	
	if(list_size(copy) == 0){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	struct Token* lparens = makeToken(LPARENS);

	if(list_starts_with(copy, lparens)){
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, LPARENS)){
			freeTokenListShallow(copy2);
			free(res);
			freeTokenListShallow(copy);
			freeToken(lparens);
			return NULL;
		}

		res->subr_type = makeSubrType(copy2);
		if(res->subr_type == NULL){
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
		
		goto end;
	} 
	
	res->simple_type = makeSimpleType(copy);
	if(res->simple_type == NULL){
		free(res);
		freeTokenListShallow(copy);
		freeToken(lparens);
		return NULL;
	}
	
	end:
	
	freeToken(lparens);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


