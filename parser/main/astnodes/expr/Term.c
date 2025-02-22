#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "Term.h"
#include "const/ConstValue.h"
#include "const/StringConst.h"
#include "Expr.h"
#include "var/Variable.h"
#include "statements/Call.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"
#include "MDirect.h"

// --- private subroutines ---

struct Term* initTerm();

// @returns false on failure
static bool tryInitExpr(struct Term* res, struct TokenList* copy);
static bool tryInitStringConst(struct Term* res, struct TokenList* copy);
// ---------------------------

struct Term* makeTerm_other(struct Expr* expr) {

	struct Term* res = initTerm();

	res->super.line_num = expr->super.line_num;
	res->super.annotations = expr->super.annotations;

	res->kind = 5;
	res->ptr.m5 = expr;
	return res;
}

struct Term* makeTerm(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct Term* res = initTerm();

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	const int tk_kind = list_head(copy)->kind;

	if (tk_kind == LPARENS) {

		if (!tryInitExpr(res, copy)) {
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}

	} else if (tk_kind == STRINGCONST) {

		if (!tryInitStringConst(res, copy)) {
			return NULL;
		}

	} else if (tk_kind == LBRACKET) {

		res->ptr.m13 = makeMDirect(copy);
		if (res->ptr.m13 == NULL) {
			goto exit_error;
		}
		res->kind = 13;

	} else {
		goto other_term;
	}

	//something matched
	goto end;

other_term:

	if ((res->ptr.m12 = makeConstValue(copy)) != NULL) {
		res->kind = 12;
		goto end;
	}

	if ((res->ptr.m4 = makeCall(copy)) != NULL) {
		res->kind = 4;
		goto end;
	}

	if ((res->ptr.m6 = makeVariable(copy)) != NULL) {
		res->kind = 6;
		goto end;
	}
exit_error:
	free(res);
	freeTokenListShallow(copy);
	return NULL;

end:
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Term* initTerm() {

	struct Term* res = make(Term);

	res->kind = 0;
	res->ptr.m5 = NULL;

	return res;
}

static bool tryInitExpr(struct Term* res, struct TokenList* copy) {

	list_consume(copy, 1);

	res->kind = 5;
	res->ptr.m5 = makeExpr(copy);
	if (res->ptr.m5 == NULL) {
		free(res);
		freeTokenListShallow(copy);
		printf("expected an Expression, but got :");
		list_print(copy);
		return false;
	}

	if (!list_expect(copy, RPARENS)) {
		//this part can be parsed deterministically
		printf("expected ')', but was: ");
		list_print(copy);
		return false;
	}

	return true;
}

static bool tryInitStringConst(struct Term* res, struct TokenList* copy) {

	res->kind = 8;
	res->ptr.m8 = makeStringConst(copy);
	if (res->ptr.m8 == NULL) {

		printf("expected an String, but got :");
		list_print(copy);

		free(res);
		freeTokenListShallow(copy);

		return false;
	}

	return true;
}
