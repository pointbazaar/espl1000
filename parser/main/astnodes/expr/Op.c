#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ast/ast/ast_expr.h"
#include "parser/main/util/parse_astnode.h"

#include "Op.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

enum OP makeOp(struct TokenList* tokens) {

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	if (tk == NULL) { return OP_NONE; }

	enum OP res = OP_NONE;

	switch (tk->kind) {
		case ASSIGNOP_PLUS:
			res = OP_PLUS;
			break;
		case ASSIGNOP_MINUS:
			res = OP_MINUS;
			break;
		case ASSIGNOP_SHIFT_LEFT:
			res = OP_SHIFT_LEFT;
			break;
		case ASSIGNOP_SHIFT_RIGHT:
			res = OP_SHIFT_RIGHT;
			break;
		case ASSIGNOP_BITWISE_AND:
			res = OP_AND;
			break;
		case ASSIGNOP_BITWISE_OR:
			res = OP_OR;
			break;

		case OPKEY_ARITHMETIC_PLUS:
			res = OP_PLUS;
			break;
		case OPKEY_ARITHMETIC_MINUS:
			res = OP_MINUS;
			break;
		case OPKEY_ARITHMETIC_MUL:
			res = OP_MULTIPLY;
			break;
		case OPKEY_ARITHMETIC_DIV:
			break;
		case OPKEY_RELATIONAL_EQ:
			res = OP_EQ;
			break;
		case OPKEY_RELATIONAL_NEQ:
			res = OP_NEQ;
			break;
		case OPKEY_RELATIONAL_LT:
			res = OP_LT;
			break;
		case OPKEY_RELATIONAL_GT:
			res = OP_GT;
			break;
		case OPKEY_RELATIONAL_LE:
			res = OP_LE;
			break;
		case OPKEY_RELATIONAL_GE:
			res = OP_GE;
			break;
		case OPKEY_LOGICAL_AND:
			res = OP_AND;
			break;
		case OPKEY_LOGICAL_OR:
			res = OP_OR;
			break;
		case OPKEY_LOGICAL_NOT:
			res = OP_NOT;
			break;
		case OPKEY_BITWISE_OR:
			res = OP_OR;
			break;
		case OPKEY_BITWISE_AND:
			res = OP_AND;
			break;
		case OPKEY_BITWISE_XOR:
			res = OP_XOR;
			break;
		case OPKEY_BITWISE_NOT:
			res = OP_NOT;
			break;
		case OPKEY_BITWISE_SHIFT_LEFT:
			res = OP_SHIFT_LEFT;
			break;
		case OPKEY_BITWISE_SHIFT_RIGHT:
			res = OP_SHIFT_RIGHT;
			break;

		default:
			freeTokenListShallow(copy);
			return OP_NONE;
	}

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
