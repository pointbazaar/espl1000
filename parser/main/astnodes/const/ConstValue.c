#include <stdio.h>
#include <stdlib.h>

#include "ConstValue.h"
#include "IntConst.h"

#include "ast/ast.h"
#include "ast/ast/ast_const.h"
#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct ConstValue* makeConstValue(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct ConstValue* res = make(ConstValue);

	struct TokenList* copy = list_copy(tokens);

	res->super.line_num = list_head(copy)->line_num;
	res->super.annotations = 0;

	const struct Token* tk = list_head(copy);

	switch (tk->kind) {

		case HEXCONST:
			//"0x10" -> 16, ...
			//use strtol to convert hex string -> int
			res->ptr.m5_hex_const = strtol(tk->value_ptr + 2, NULL, 16);
			res->kind = 5;
			list_consume(copy, 1);
			break;

		case BINCONST:

			//"0b10" -> 2, ...
			//use strtol to convert bin string -> int
			res->ptr.m5_hex_const = strtol(tk->value_ptr + 2, NULL, 2);
			res->kind = 6;
			list_consume(copy, 1);
			break;

		case CCONST:
			//index needs to be 1, as charconst
			//is surrounded by single quotes
			res->ptr.m3_char_const = tk->value_ptr[1];
			res->kind = 3;
			list_consume(copy, 1);
			break;

		case BCONST_FALSE:
			res->ptr.m1_bool_const = false;
			res->kind = 1;
			list_consume(copy, 1);
			break;

		case BCONST_TRUE:
			res->ptr.m1_bool_const = true;
			res->kind = 1;
			list_consume(copy, 1);
			break;

		default: {
			//try to parse IntConst
			bool error = false;
			res->ptr.m2_int_const = makeIntConst(copy, &error);

			if (error) {
				free_const_value(res);
				freeTokenListShallow(copy);
				return NULL;
			}

			res->kind = 2;
			break;
		}
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
