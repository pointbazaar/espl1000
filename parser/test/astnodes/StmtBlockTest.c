#include "StmtBlockTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/statements/Stmt.h"
#include "../../main/parsing/StmtBlock.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>

int test_stmtblock_1(bool debug){
	
	if(debug){
		printf("TEST: test_stmtblock_1\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(LCURLY,"{"));
	
	list_add(tokens, makeToken2(ID,"a") );
	list_add(tokens, makeToken2(ASSIGNOP,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken(SEMICOLON) );
	
	list_add(tokens, makeToken2(RCURLY,"}"));

	struct StmtBlock* node = makeStmtBlock(tokens,debug);

	bool assert1 = (0 == list_size(tokens)); 
	//all tokens should have been consumed
	
	bool assert2 = node != NULL;
	
	freeTokenList(tokens);
	freeStmtBlock(node);

	return (assert1 && assert2)?1:0;
}
