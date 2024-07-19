#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "PrimitiveType.h"

#include "ast/ast.h"
#include "ast/sd_inttype.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct PrimitiveType* makePrimitiveType(struct TokenList* tokens) {

	struct PrimitiveType* res = make(PrimitiveType);

	res->int_type = NONE;
	res->is_int_type = false;
	res->is_bool_type = false;
	res->is_char_type = false;

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* next = list_head(copy);

	uint32_t kind = next->kind;

	switch (kind) {

		case TYPEID_PRIMITIVE_INT:
			res->is_int_type = true;
			res->int_type = INT;
			break;
		case TYPEID_PRIMITIVE_UINT:
			res->is_int_type = true;
			res->int_type = UINT;
			break;
		case TYPEID_PRIMITIVE_INT8:
			res->is_int_type = true;
			res->int_type = INT8;
			break;
		case TYPEID_PRIMITIVE_UINT8:
			res->is_int_type = true;
			res->int_type = UINT8;
			break;
		case TYPEID_PRIMITIVE_INT16:
			res->is_int_type = true;
			res->int_type = INT16;
			break;
		case TYPEID_PRIMITIVE_UINT16:
			res->is_int_type = true;
			res->int_type = UINT16;
			break;

		case TYPEID_PRIMITIVE_BOOL:
			res->is_bool_type = true;
			break;

		case TYPEID_PRIMITIVE_CHAR:
			res->is_char_type = true;
			break;

		default:
			free(res);
			freeTokenListShallow(copy);
			return NULL;
	}

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
