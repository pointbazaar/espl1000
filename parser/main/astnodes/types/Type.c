#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Type.h"
#include "ArrayType.h"
#include "PointerType.h"
#include "TypeParam.h"
#include "BasicType.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Type* makeType_1(struct BasicType* typeNode) {

	struct Type* res = make(Type);

	res->super.line_num = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = typeNode;
	res->type_param = NULL;
	res->array_type = NULL;

	return res;
}

struct Type* makeType_2(struct TypeParam* typeNode) {

	struct Type* res = make(Type);

	res->super.line_num = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = NULL;
	res->type_param = typeNode;
	res->array_type = NULL;

	return res;
}

struct Type* makeType_3(struct ArrayType* typeNode) {

	struct Type* res = make(Type);

	res->super.line_num = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = typeNode;

	return res;
}

struct Type* makeType_4(struct SimpleType* typeNode) {

	struct Type* res = make(Type);

	res->super.line_num = typeNode->super.line_num;
	res->super.annotations = 0;

	res->basic_type = makeBasicTypeSimple(typeNode);

	return res;
}

struct Type* makeType2(struct TokenList* tokens) {

	// each type has at least 1 token
	if (list_size(tokens) == 0) {
		return NULL;
	}

	struct Type* res = make(Type);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = NULL;
	res->pointer_type = NULL;
	res->is_anytype = false;

	struct Token* head = list_head(copy);

	switch (head->kind) {
		case OPKEY_ARITHMETIC_MUL:
			res->pointer_type = makePointerType2(copy);
			if (res->pointer_type != NULL) { goto end; }
			break;
		case LBRACKET:
			res->array_type = makeArrayType2(copy);
			if (res->array_type != NULL) { goto end; }
			break;
		case TPARAM:
			res->type_param = makeTypeParam(copy);
			if (res->type_param != NULL) { goto end; }
			break;
		case ANYTYPE:
			list_consume(copy, 1);
			res->is_anytype = true;
			goto end;
		default:
			res->basic_type = makeBasicType2(copy);
			if (res->basic_type != NULL) { goto end; }
			break;
	}

	//nothing matched
	free(res);
	freeTokenListShallow(copy);
	return NULL;

end:
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
