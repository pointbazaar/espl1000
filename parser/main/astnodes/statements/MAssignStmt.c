#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "statements/MAssignStmt.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"
#include "expr/MDirect.h"

struct MAssignStmt* makeMAssignStmt(struct TokenList* tokens) {

	struct MAssignStmt* res = make(MAssignStmt);

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->lhs = makeMDirect(copy);

	//parse '='
	struct Token* head = list_head(copy);

	if (
	    head->kind != ASSIGNOP_PLUS &&
	    head->kind != ASSIGNOP_MINUS &&
	    head->kind != ASSIGNOP_SHIFT_LEFT &&
	    head->kind != ASSIGNOP_SHIFT_RIGHT &&
	    head->kind != ASSIGNOP_BITWISE_AND &&
	    head->kind != ASSIGNOP_BITWISE_OR &&
	    head->kind != ASSIGNOP_SIMPLE) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if (head->kind != ASSIGNOP_SIMPLE) {
		fprintf(stderr, "expected assign op to be '=' for MAssignStmt. Exiting\n");
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_consume(copy, 1);

	res->expr = makeExpr(copy);

	//parse ';'
	head = list_head(copy);
	if (head->kind != SEMICOLON) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
