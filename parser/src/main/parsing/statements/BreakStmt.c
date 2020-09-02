#include <stdio.h>
#include <stdlib.h>

#include "../StmtBlock.h"
#include "BreakStmt.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "Stmt.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct BreakStmt* makeBreakStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("BreakStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, BREAK)){
		freeTokenListShallow(copy);
		return NULL;
	}

	if(!list_expect(copy, SEMICOLON)){
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing BreakStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	struct BreakStmt* res = smalloc(sizeof(struct BreakStmt));

	return res;
}


