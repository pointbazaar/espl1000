#include <stdio.h>
#include <stdlib.h>

#include "CaseStmt.h"
#include "../IntConst.h"
#include "../CharConst.h"
#include "../BoolConst.h"
#include "../StmtBlock.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../../../../util/util.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"


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
	
	struct CaseStmt* res = smalloc(sizeof(struct CaseStmt));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	
	res->block = NULL;

	if((res->m1 = makeBoolConst(copy,debug)) == NULL){
		if((res->m2 = makeCharConst(copy,debug)) == NULL){
			if((res->m3 = makeIntConst(copy,debug)) == NULL){
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

