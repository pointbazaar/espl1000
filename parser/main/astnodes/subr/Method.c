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

	res->block = makeStmtBlock(copy);
	if (res->block == NULL) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
