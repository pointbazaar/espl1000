#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SimpleVar.h"
#include "Variable.h"
#include "ast/ast.h"
#include "parser/main/util/parse_astnode.h"
#include "token/TokenKeys.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Variable* makeVariable(struct TokenList* tokens) {

	if(list_size(tokens) < 1) {
		return NULL;
	}

	struct Variable*  res  = make(Variable);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->member_access = NULL;

	res->simple_var = makeSimpleVar(copy);

	if(res->simple_var == NULL) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	if(list_size(copy) < 2) {
		goto end;
	}

	struct Token* next = list_head(copy);

	if(next->kind != STRUCTMEMBERACCESS) {
		goto end;
	}

	list_expect(copy, STRUCTMEMBERACCESS);

	res->member_access = makeVariable(copy);

	if(res->member_access == NULL) {

		printf("[Parser[Error] parsing struct member access!\n");
		printf("%s\n", list_code(copy));
		free(res);
		freeTokenListShallow(copy);
		exit(1);
	}

end:

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
