#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "CaseStmt.h"

#include "const/IntConst.h"
#include "const/CharConst.h"
#include "const/BoolConst.h"

#include "StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct CaseStmt* makeCaseStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("CaseStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 4){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, CASE)){
		return NULL;
	}
	
	struct CaseStmt* res = make(CaseStmt);
	
	parse_astnode(copy, &(res->super));
	
	res->kind = 0;
	res->ptr.m1 = NULL;
	res->block = NULL;

	res->kind = 0;
	if((res->ptr.m1 = makeBoolConst(copy,debug)) == NULL){
		res->kind = 1;
		if((res->ptr.m2 = makeCharConst(copy,debug)) == NULL){
			res->kind = 2;
			if((res->ptr.m3 = makeIntConst(copy,debug)) == NULL){
				free(res);
				
				//parsing is deterministic here, 
				//so it must be one of these values
				printf("Error: no constant case value given\n");
				list_print(copy);
				exit(1);
				return NULL;
			}
		}
	}

	//the block is optional
	//and can be NULL
	res->block = makeStmtBlock(copy, debug);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

