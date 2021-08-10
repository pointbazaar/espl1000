#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Type.h"
#include "ArrayType.h"
#include "TypeParam.h"
#include "BasicType.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Type* makeType_1(struct BasicType* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = typeNode;
	res->type_param = NULL;
	res->array_type = NULL;

	return res;
}

struct Type* makeType_2(struct TypeParam* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = NULL;
	res->type_param = typeNode;
	res->array_type = NULL;

	return res;
}

struct Type* makeType_3(struct ArrayType* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = typeNode;

	return res;
}

struct Type* makeType2(struct TokenList* tokens) {

	struct Type* res = make(Type);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = NULL;

	res->array_type     = makeArrayType2(copy);
	if(res->array_type != NULL){ goto end; }
		
		
	res->type_param     = makeTypeParam(copy);
	if(res->type_param != NULL) { goto end; }
	
	res->basic_type     = makeBasicType2(copy);
	if(res->basic_type != NULL){ goto end; }
	
	//nothing matched
	free(res);
	freeTokenListShallow(copy);
	return NULL;
	
	end:
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


