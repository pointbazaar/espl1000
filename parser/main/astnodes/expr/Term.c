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

	res->kind = TERM_KIND_EXPR;
	res->ptr.expr_term = expr;
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

	} else {
		goto other_term;
	}

	//something matched
	goto end;

other_term:

	if ((res->ptr.constvalue_term = makeConstValue(copy)) != NULL) {
		res->kind = TERM_KIND_CONSTVALUE;
		goto end;
	}

	if ((res->ptr.call_term = makeCall(copy)) != NULL) {
		res->kind = TERM_KIND_CALL;
		goto end;
	}

	if ((res->ptr.var_term = makeVariable(copy)) != NULL) {
		res->kind = TERM_KIND_VAR;
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
	res->ptr.expr_term = NULL;

	return res;
}

static bool tryInitExpr(struct Term* res, struct TokenList* copy) {

	list_consume(copy, 1);

	res->kind = TERM_KIND_EXPR;
	res->ptr.expr_term = makeExpr(copy);
	if (res->ptr.expr_term == NULL) {
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

	res->kind = TERM_KIND_STRINGCONST;
	res->ptr.stringconst_term = makeStringConst(copy);
	if (res->ptr.stringconst_term == NULL) {

		printf("expected an String, but got :");
		list_print(copy);

		free(res);
		freeTokenListShallow(copy);

		return false;
	}

	return true;
}
