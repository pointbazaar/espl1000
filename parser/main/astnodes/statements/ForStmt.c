#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "ForStmt.h"
#include "Range.h"
#include "StmtBlock.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct ForStmt* makeForStmt(struct TokenList* tokens) {

	if (list_size(tokens) < 6) { return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if (!list_expect(copy, FOR)) {
		return NULL;
	}

	struct Token* head = list_head(copy);

	if (head->kind != ID) {
		freeTokenListShallow(copy);
		return NULL;
	}

	list_consume(copy, 1);

	struct ForStmt* res = make(ForStmt);

	parse_astnode(copy, &(res->super));

	//copy the index Name
	strncpy(res->index_name, head->value_ptr, DEFAULT_STR_SIZE);

	if (!list_expect(copy, IN)) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	res->range = makeRange(copy);

	if (res->range == NULL) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	res->block = makeStmtBlock(copy);

	if (res->block == NULL) {
		freeTokenListShallow(copy);
		free_range(res->range);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
