#include "StmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/statements/Stmt.hpp"

int stmt_test_assignment_statement_with_struct_access() {

	TokenList tokens = TokenList();
	tokens.add(ID,"x");
	tokens.add(STRUCTMEMBERACCESS);
	tokens.add(ID,"a");
	tokens.add(OPKEY,"=");
	tokens.add(INTEGER,"3");
	tokens.add(SEMICOLON);

	Stmt* node = new Stmt(tokens,false);

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

	Stmt* node = new Stmt(tokens,false);

	bool assert1 = (0 == tokens.size()); //all tokens should have been consumed

	return (assert1)?1:0;
}
