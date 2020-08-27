#include <stdio.h>
#include <stdlib.h>

#include "../StmtBlock.h"
#include "BreakStmt.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "Stmt.h"
#include "../../../../../util/util.h"

struct BreakStmt* makeBreakStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BreakStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL; }

	struct BreakStmt* res = smalloc(sizeof(struct BreakStmt));

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, BREAK)){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, SEMICOLON)){
		free(res);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing BreakStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeBreakStmt(struct BreakStmt* b){
	
	free(b);
}

