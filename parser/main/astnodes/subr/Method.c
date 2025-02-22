#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Method.h"
#include "StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"
#include "MethodDecl.h"

struct Method* makeMethod(struct TokenList* tokens) {

	struct Method* res = make(Method);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->decl = makeMethodDecl(copy);
	if (!res->decl) {
		goto err_decl;
	}

	res->block = makeStmtBlock(copy);
	if (res->block == NULL) {
		goto err_block;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

err_block:
	free_method_decl(res->decl);
err_decl:
	freeTokenListShallow(copy);
	free(res);

	return NULL;
}
