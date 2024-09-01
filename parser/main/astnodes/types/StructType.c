#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "StructType.h"
#include "Type.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct StructType* makeStructType(struct TokenList* tokens) {

	struct StructType* res = make(StructType);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* next = list_head(copy);

	if (next->kind != TYPEID && next->kind != ANYTYPE) {

		printf("[Parser][Error] Could not parse StructType\n");
		exit(1);
		return NULL;
	}

	asprintf(&(res->type_name), "%s", next->value_ptr);

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
