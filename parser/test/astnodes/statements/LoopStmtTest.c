#include "LoopStmtTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/astnodes/statements/LoopStmt.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>
#include <assert.h>

int loop_test1(bool debug){
	
	if(debug){
		printf("TEST: loop_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(LOOP,"LOOP"));
	list_add(list, makeToken2(INTEGER,"3"));
	list_add(list, makeToken2(LCURLY,"{"));
	
		list_add(list, makeToken2(RETURN,"return"));
		list_add(list, makeToken2(INTEGER,"3"));
		list_add(list, makeToken2(SEMICOLON,";"));
	
	list_add(list, makeToken2(RCURLY,"}"));

	struct LoopStmt* node = makeLoopStmt(list,debug);
	
	assert(node != NULL);
	
	freeTokenList(list);
	freeLoopStmt(node);

	return 1;
}
