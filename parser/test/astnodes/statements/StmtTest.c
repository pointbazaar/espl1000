#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "StmtTest.h"

#include "statements/Stmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int stmt_test_assignment_statement_with_struct_access(bool debug) {

	if(debug){
		printf("TEST: stmt_test_assignment_statement_with_struct_access\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,"."));
	list_add(tokens, makeToken2(ID,"a") );
	list_add(tokens, makeToken2(ASSIGNOP,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken2(SEMICOLON, ";"));

	struct Stmt* node = makeStmt(tokens,debug);

	//all tokens should have been consumed
	assert(0 == list_size(tokens)); 
	assert(node != NULL);
	
	freeTokenList(tokens);
	free_stmt(node);

	return 1;
}

int stmt_test_assignment_statement_with_method_call(bool debug) {

	if(debug){
		printf("TEST: stmt_test_assignment_statement_with_method_call\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"flip") );
	list_add(tokens, makeToken(LPARENS) );
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken(RPARENS) );
	list_add(tokens, makeToken(SEMICOLON) );

	struct Stmt* node = makeStmt(tokens,debug);

	assert(0 == list_size(tokens)); //all tokens should have been consumed
	assert(node != NULL);
	
	freeTokenList(tokens);
	free_stmt(node);

	return 1;
}
