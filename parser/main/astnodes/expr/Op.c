#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "Op.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

enum OP makeOp(struct TokenList* tokens) {

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	if (tk == NULL) { return OP_NONE; }

	switch (tk->kind) {

		case OPKEY_ARITHMETIC:
		case OPKEY_RELATIONAL:
		case OPKEY_LOGICAL:
		case OPKEY_BITWISE: break;

		default:
			freeTokenListShallow(copy);
			return OP_NONE;
	}

	enum OP res = OP_NONE;

	if (strcmp(tk->value_ptr, "+") == 0) res = OP_PLUS;
	if (strcmp(tk->value_ptr, "-") == 0) res = OP_MINUS;
	if (strcmp(tk->value_ptr, "*") == 0) res = OP_MULTIPLY;

	if (strcmp(tk->value_ptr, "<<") == 0) res = OP_SHIFT_LEFT;
	if (strcmp(tk->value_ptr, ">>") == 0) res = OP_SHIFT_RIGHT;

	if (strcmp(tk->value_ptr, "==") == 0) res = OP_EQ;
	if (strcmp(tk->value_ptr, "!=") == 0) res = OP_NEQ;
	if (strcmp(tk->value_ptr, ">=") == 0) res = OP_GE;
	if (strcmp(tk->value_ptr, "<=") == 0) res = OP_LE;
	if (strcmp(tk->value_ptr, ">") == 0) res = OP_GT;
	if (strcmp(tk->value_ptr, "<") == 0) res = OP_LT;

	if (strcmp(tk->value_ptr, "&") == 0) res = OP_AND;
	if (strcmp(tk->value_ptr, "|") == 0) res = OP_OR;
	if (strcmp(tk->value_ptr, "^") == 0) res = OP_XOR;

	if (strcmp(tk->value_ptr, "!") == 0) res = OP_NOT;
	if (strcmp(tk->value_ptr, "~") == 0) res = OP_COMPLEMENT;

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
