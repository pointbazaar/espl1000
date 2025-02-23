#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StmtBlock.h"
#include "statements/Stmt.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct StmtBlock* makeStmtBlock(struct TokenList* tokens) {

	if (list_size(tokens) < 2) { return NULL; }

	struct StmtBlock* res = make(StmtBlock);

	if (!res) {
		return NULL;
	}

	res->super.line_num = list_head(tokens)->line_num;
	res->super.annotations = 0;

	res->count = 0;
	res->stmts = malloc(sizeof(struct Stmt*) * 1);

	if (!res->stmts) {
		goto err_stmts_malloc;
	}

	struct TokenList* copy = list_copy(tokens);

	if (!list_expect(copy, LCURLY)) {
		goto err_lcurly;
	}

	struct Token* next = list_head(copy);

	while (next->kind != RCURLY) {

		struct Stmt* stmt = makeStmt(copy);

		if (stmt == NULL) {
			goto err_stmts;
		}

		res->stmts[res->count] = stmt;
		res->count++;

		const int newsize = sizeof(struct Stmt*) * (res->count + 1);
		res->stmts = realloc(res->stmts, newsize);

		next = list_head(copy);
		if (next == NULL) {
			goto err_stmts;
		}
	}

	if (!list_expect(copy, RCURLY)) {
		goto err_rcurly;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

err_rcurly:
err_stmts:
	for (int i = 0; i < res->count; i++) {
		free_stmt(res->stmts[i]);
	}
	free(res->stmts);
err_lcurly:
	freeTokenListShallow(copy);
err_stmts_malloc:
	free(res);
	return NULL;
}
