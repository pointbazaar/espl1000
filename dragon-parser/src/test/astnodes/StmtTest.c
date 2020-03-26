#include "StmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/Stmt.h"

int stmt_test_assignment_statement_with_struct_access() {

	TokenList tokens = TokenList();
	tokens.add(ID,"x");
	tokens.add(STRUCTMEMBERACCESS);
	tokens.add(ID,"a");
	tokens.add(OPKEY,"=");
	tokens.add(INTEGER,"3");
	tokens.add(SEMICOLON);

	struct Stmt* node = makeStmt(&tokens,false);

	bool assert1 = (0 == tokens.size()); //all tokens should have been consumed
	return (assert1)?1:0;
}

int stmt_test_assignment_statement_with_method_call() {

	TokenList tokens = TokenList();

	tokens.add(ID,"flip");
	tokens.add(LPARENS);
	tokens.add(ID,"x");
	tokens.add(RPARENS);
	tokens.add(SEMICOLON);

	struct Stmt* node = makeStmt(&tokens,false);

	bool assert1 = (0 == tokens.size()); //all tokens should have been consumed

	return (assert1)?1:0;
}
