#include <stdlib.h>
#include <string.h>

#include "Method.h"
#include "MethodDecl.h"
#include "StmtBlock.h"
#include "ast/util/free_ast.h"
#include "parser/main/util/parse_astnode.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Method* makeMethod(struct TokenList* tokens) {
	struct Method*    res  = make(Method);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->decl = makeMethodDecl(copy);

	res->block = makeStmtBlock(copy);
	if(res->block == NULL) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
