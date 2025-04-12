#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "EnumMember.h"
#include "Identifier.h"
#include "types/Type.h"
#include "types/StructType.h"
#include "const/ConstValue.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct EnumMember* makeEnumMember(struct TokenList* tokens) {

	struct EnumMember* res = make(EnumMember);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (list_head(copy)->kind != TOKEN_ENUM_VALUE) {

		fprintf(stderr, "expected enum value (all uppercase)\n");
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	asprintf(&(res->name), "%s", list_head(copy)->value_ptr);
	list_consume(copy, 1);

	struct Token* next = list_head(copy);
	if (next->kind != ASSIGNOP_SIMPLE) {
		fprintf(stderr, "expected '='\n");
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	list_consume(copy, 1);

	res->value = makeConstValue(copy);

	if (!res->value) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	next = list_head(copy);
	if (next->kind != SEMICOLON) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
