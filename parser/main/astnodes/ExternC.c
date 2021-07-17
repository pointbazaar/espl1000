#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr/MethodDecl.h"
#include "const/StringConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ExternC.h"

struct ExternC* makeExternC(struct TokenList* tokens){

	struct ExternC* res = make(ExternC);

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, EXTERNC)){
		printf("expected 'externc', but was: %s\n", list_code(copy));
		free(res);
		return NULL;
	}

	res->decl = makeMethodDecl(copy);
	if (res->decl == NULL){ return NULL; }

	struct StringConst* string_const = makeStringConst(copy);
	if (string_const == NULL){ return NULL; }

	strncpy(res->name_in_c, string_const->value, DEFAULT_STR_SIZE-1);
	free_string_const(string_const);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}