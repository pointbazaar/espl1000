#include "BreakStmtTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/astnodes/statements/BreakStmt.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>
#include <assert.h>

int break_test1(bool debug){
	
	if(debug){
		printf("TEST: break_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(BREAK,"BREAK"));
	list_add(list, makeToken2(SEMICOLON,";"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct BreakStmt* node = makeBreakStmt(list,debug);
	
	assert(node != NULL);
	
	freeTokenList(list);
	freeBreakStmt(node);

	return 1;
}
