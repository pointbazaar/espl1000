#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Identifier.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct Id* makeIdentifier(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct Id* res = make(Id);

	struct Token* tk = list_head(tokens);

	res->super.line_num = tk->line_num;
	res->super.annotations = 0;

	if (tk->kind == ID) {
		strncpy(res->identifier, tk->value_ptr, DEFAULT_STR_SIZE - 1);
		list_consume(tokens, 1);

		res->identifier[DEFAULT_STR_SIZE - 1] = '\0';
	} else {
		free(res);
		return NULL;
	}

	return res;
}
