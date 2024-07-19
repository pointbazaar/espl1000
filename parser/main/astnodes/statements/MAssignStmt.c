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

	if (head->kind != ASSIGNOP) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if (strcmp(head->value_ptr, "=") != 0) {
		printf("expected assign op to be '=' for MAssignStmt. Exiting\n");
		exit(1);
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
