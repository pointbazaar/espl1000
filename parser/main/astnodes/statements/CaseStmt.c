#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "CaseStmt.h"

#include "const/ConstValue.h"

#include "StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct CaseStmt* makeCaseStmt(struct TokenList* tokens) {
	
	if(list_size(tokens) < 4){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, CASE)){
		return NULL;
	}
	
	struct CaseStmt* res = make(CaseStmt);
	
	parse_astnode(copy, &(res->super));

	res->const_value = makeConstValue(copy);

	if (res->const_value == NULL) {

		printf("[Parser][Error] no constant case value given\n");
		list_print(copy);
		exit(1);
		return NULL;
	}

	//the block is optional and can be NULL
	res->block = makeStmtBlock(copy);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

