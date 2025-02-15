#include <stdio.h>
#include <stdlib.h>

#include "const/IntConst.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "MDirect.h"
#include "parse_astnode.h"

const char* PARSE_ERR_MDIRECT = "Failed to parse MDirect. Exiting\n";

struct MDirect* makeMDirect(struct TokenList* tokens) {

	struct MDirect* res = make(MDirect);

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* head = list_head(copy);

	if (head->kind != LBRACKET) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	list_consume(copy, 1);

	res->expr = makeExpr(copy);

	if (res->expr == NULL) {
		fprintf(stderr, "expected expr, got %s\n", list_code(copy));
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	if (!list_expect(copy, COMMA)) {
		fprintf(stderr, "expected ',', got %s\n", list_code(copy));
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	bool error = false;
	res->load_store_width = makeIntConst(copy, &error);

	if (error) {
		fprintf(stderr, "expected int const, got %s\n", list_code(copy));
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	head = list_head(copy);

	if (head->kind != RBRACKET) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
