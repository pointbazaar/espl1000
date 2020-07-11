#include "StmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/Stmt.h"

#include <stdbool.h>
#include <stdio.h>

int stmt_test_assignment_statement_with_struct_access(bool debug) {

	if(debug){
		printf("TEST: stmt_test_assignment_statement_with_struct_access\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken(STRUCTMEMBERACCESS) );
	list_add(tokens, makeToken2(ID,"a") );
	list_add(tokens, makeToken2(EQ,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken(SEMICOLON) );

	struct Stmt* node = makeStmt(tokens,debug);

	bool assert1 = (0 == list_size(tokens)); //all tokens should have been consumed
	bool assert2 = node != NULL;

	return (assert1 && assert2)?1:0;
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

	bool assert1 = (0 == list_size(tokens)); //all tokens should have been consumed
	bool assert2 = node != NULL;

	return (assert1 && assert2)?1:0;
}
