#include "BreakStmtTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/statements/BreakStmt.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdio.h>

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
	
	const bool a1 = node != NULL;
	
	freeTokenList(list);
	freeBreakStmt(node);

	return (a1)?1:0;
}
