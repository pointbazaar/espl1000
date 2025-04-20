#define _GNU_SOURCE

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
		asprintf(&(res->identifier), "%s", tk->value_ptr);
		list_consume(tokens, 1);
	} else {
		free(res);
		return NULL;
	}

	return res;
}
