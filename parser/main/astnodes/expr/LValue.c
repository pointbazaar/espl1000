#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Deref.h"
#include "../var/Variable.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct LValue* makeLValue(struct Variable* var) {

	struct LValue* res = make(LValue);

	res->super.line_num = var->super.line_num;
	res->super.annotations = 0;

	res->var = var;
	res->deref = NULL;

	return res;
}

struct LValue* makeLValue2(struct TokenList* tokens) {

	if (list_size(tokens) == 0) {
		return NULL;
	}

	struct LValue* res = make(LValue);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* head = list_head(copy);

	if (head->kind == OPKEY_ARITHMETIC_MUL) {

		res->deref = makeDeref2(copy);
		if (res->deref == NULL) {
			goto error;
		}
	} else {
		res->var = makeVariable(copy);
		if (res->var == NULL) {
			goto error;
		}
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);
	return res;
error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}
