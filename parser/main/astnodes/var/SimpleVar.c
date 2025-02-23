#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "SimpleVar.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct SimpleVar* makeSimpleVar(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct SimpleVar* res = make(SimpleVar);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->count_indices = 0;
	res->indices = malloc(sizeof(struct Expr*) * 1);

	struct Token* token = list_head(copy);

	if (token->kind != ID) {
		goto error_indices;
	}

	asprintf(&(res->name), "%s", token->value_ptr);
	list_consume(copy, 1);

	//it could have an index
	while (list_size(copy) > 0 && list_head(copy)->kind == LBRACKET) {

		if (!list_expect(copy, LBRACKET)) {
			goto error_name;
		}
		res->indices[res->count_indices] = makeExpr(copy);
		res->count_indices += 1;
		res->indices =
		    realloc(
			res->indices,
			sizeof(struct Expr*) * (res->count_indices + 1));

		if (res->indices[res->count_indices - 1] == NULL) {
			goto error_name;
		}

		if (!list_expect(copy, RBRACKET)) {
			goto error_name;
		}
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

error_name:
	free(res->name);
error_indices:
	for (size_t i = 0; i < res->count_indices; i++) {
		free_expr(res->indices[i]);
	}
	free(res->indices);
error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}
