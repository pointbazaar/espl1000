#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "struct/StructDecl.h"
#include "subr/MethodDecl.h"
#include "const/StringConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ExternC.h"

struct ExternC* makeExternC(struct TokenList* tokens){

	struct ExternC* res = make(ExternC);
	*res = (struct ExternC){
		.struct_decl = NULL,
		.subr_decl = NULL
	} ;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, EXTERNC)){
		printf("expected 'externc', but was: %s\n", list_code(copy));
		free(res);
		return NULL;
	}

	if (list_head(copy)->kind == FN) {

		res->subr_decl = makeMethodDecl(copy);
		if (res->subr_decl == NULL) {
			printf("[Parser][Error] Expected subroutine declaration.\n");
			exit(1);
			return NULL;
		}
	}else if (list_head(copy)->kind == STRUCT){

		res->struct_decl = makeStructDecl(copy);
		if (res->struct_decl == NULL){
			printf("[Parser][Error] Expected struct declaration.\n");
			exit(1);
			return NULL;
		}
	}else{

		printf("[Parser][Error] Expected 'fn' or 'struct' following 'externc' \n");
		printf("[Parser] But got: \n");
		list_print(copy);
		exit(1);
	}

	struct StringConst* string_const = makeStringConst(copy);
	if (string_const == NULL){ return NULL; }

	strncpy(res->name_in_c, string_const->value, DEFAULT_STR_SIZE-1);
	free_string_const(string_const);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}