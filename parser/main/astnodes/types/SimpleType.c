#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "SimpleType.h"

#include "PrimitiveType.h"
#include "StructType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct SimpleType* makeSimpleType(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct SimpleType* res = make(SimpleType);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->primitive_type = NULL;
	res->struct_type = NULL;

	struct Token* next = list_head(copy);

	switch (next->kind) {

		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_UINT:
		case TYPEID_PRIMITIVE_INT8:
		case TYPEID_PRIMITIVE_UINT8:
		case TYPEID_PRIMITIVE_INT16:
		case TYPEID_PRIMITIVE_UINT16:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:

			res->primitive_type = makePrimitiveType(copy);
			break;

		case TYPEID:
		case ANYTYPE:

			res->struct_type = makeStructType(copy);
			break;

		default:
			free(res);
			freeTokenListShallow(copy);
			return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
