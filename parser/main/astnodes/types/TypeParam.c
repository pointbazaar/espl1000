#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TypeParam.h"
#include "ast/ast.h"
#include "parser/main/util/parse_astnode.h"
#include "token/TokenKeys.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"

struct TypeParam* makeTypeParam(struct TokenList* tokens) {

	if(list_size(tokens) == 0) {
		return NULL;
	}

	struct TypeParam* res  = make(TypeParam);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* token = list_head(copy);

	if(token->kind != TPARAM) {
		// Error: could not read type parameter node
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	res->index = atoi(token->value_ptr);

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
