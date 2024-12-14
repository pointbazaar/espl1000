#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "SubrType.h"
#include "Type.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct SubrType* makeSubrType(struct TokenList* tokens) {

	struct SubrType* res = make(SubrType);

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->arg_types = malloc(sizeof(struct Type*) * 1);
	res->count_arg_types = 0;

	if (!list_expect(copy, LPARENS)) {
		freeTokenListShallow(copy);
		free(res->arg_types);
		free(res);
		return NULL;
	}

	bool sucess_argument_types = true;

	bool fail = false;
	struct Type* mytype = makeType2(copy);
	if (mytype == NULL) { fail = true; }

	if (!fail) {
		res->arg_types[res->count_arg_types++] = mytype;
		res->arg_types = realloc(res->arg_types, sizeof(struct Type*) * (res->count_arg_types + 1));

	} else {
		sucess_argument_types = false;
	}

	while (sucess_argument_types) {
		bool fail2 = false;

		struct TokenList* copy2 = list_copy(copy);

		if (!list_expect(copy2, COMMA)) { fail2 = true; }

		if (!fail2) {
			struct Type* mytype2 = makeType2(copy2);
			if (mytype2 == NULL) { fail2 = true; }

			if (!fail2) {
				res->arg_types[res->count_arg_types] = mytype2;
				res->count_arg_types++;

				size_t newsize = sizeof(struct Type*) * (res->count_arg_types + 1);
				res->arg_types = realloc(res->arg_types, newsize);

				list_set(copy, copy2);
			}
		} else {
			sucess_argument_types = false;
		}

		freeTokenListShallow(copy2);
	}

	if (!list_expect(copy, RPARENS)) {
		freeTokenListShallow(copy);
		free(res->arg_types);
		free(res);
		return NULL;
	}

	{
		struct Token* tk = list_head(copy);
		if (tk->kind != ARROW_SIDE_EFFECT && tk->kind != ARROW_NO_SIDE_EFFECT) {
			free(res->arg_types);
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}

		res->has_side_effects = (tk->kind == ARROW_SIDE_EFFECT);

		list_consume(copy, 1);
	}

	res->return_type = makeType2(copy);
	if (res->return_type == NULL) {
		freeTokenListShallow(copy);
		free(res->arg_types);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
