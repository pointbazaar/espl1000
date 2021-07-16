#include <stdio.h>
#include <stdlib.h>

#include "ConstValue.h"

#include "BinConst.h"
#include "CharConst.h"
#include "HexConst.h"
#include "BoolConst.h"
#include "FloatConst.h"
#include "IntConst.h"

#include "ast/ast.h"
#include "ast/ast/ast_const.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct ConstValue* makeConstValue(struct TokenList* tokens){

	if(list_size(tokens) == 0){return NULL;}

	struct ConstValue* res = make(ConstValue);

	struct TokenList* copy = list_copy(tokens);

	res->super.line_num    = list_head(copy)->line_num;
	res->super.annotations = 0;

	const int tk_kind = list_head(copy)->kind;

	switch (tk_kind) {
		case HEXCONST:
			res->ptr.m5_hex_const = makeHexConst(copy);
			res->kind = 5;
			break;

		case BINCONST:
			res->ptr.m6_bin_const = makeBinConst(copy);
			res->kind = 6;
			break;

		case CCONST:
			res->ptr.m3_char_const = makeCharConst(copy);
			res->kind = 3;
			break;

		case BCONST_FALSE:
		case BCONST_TRUE:
			res->ptr.m1_bool_const = makeBoolConst(copy);
			res->kind = 1;
			break;

		default:
			goto other_term;
	}

	goto end; //something matched

	other_term:

	if((res->ptr.m2_int_const = makeIntConst(copy)) != NULL){
		res->kind = 2;
		goto end;
	}

	if((res->ptr.m4_float_const = makeFloatConst(copy)) != NULL){
		res->kind = 4;
		goto end;
	}

	free(res);
	freeTokenListShallow(copy);
	return NULL;

	end:

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}