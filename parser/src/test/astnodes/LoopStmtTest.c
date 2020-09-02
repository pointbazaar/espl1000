#include "LoopStmtTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/statements/LoopStmt.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>

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
	
	const bool a1 = node != NULL;
	
	freeTokenList(list);
	freeLoopStmt(node);

	return (a1)?1:0;
}
