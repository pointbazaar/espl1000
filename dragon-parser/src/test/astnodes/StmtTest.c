#include "StmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/Stmt.h"

#include <stdbool.h>

int stmt_test_assignment_statement_with_struct_access() {

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken(STRUCTMEMBERACCESS) );
	list_add(tokens, makeToken2(ID,"a") );
	list_add(tokens, makeToken2(OPKEY,"=") );
	list_add(tokens, makeToken2(INTEGER,"3") );
	list_add(tokens, makeToken(SEMICOLON) );

	struct Stmt* node = makeStmt(tokens,false);

	bool assert1 = (0 == list_size(tokens)); //all tokens should have been consumed
	return (assert1)?1:0;
}

int stmt_test_assignment_statement_with_method_call() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"flip") );
	list_add(tokens, makeToken(LPARENS) );
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken(RPARENS) );
	list_add(tokens, makeToken(SEMICOLON) );

	struct Stmt* node = makeStmt(tokens,false);

	bool assert1 = (0 == list_size(tokens)); //all tokens should have been consumed

	return (assert1)?1:0;
}