#include "IfStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/statements/IfStmt.hpp"

int if_test1() {
	TokenList list = new TokenList();
	list.add(IF);

	list.add(LPARENS);

	list.add(INTEGER, "5");
	list.add(OPKEY, "<");
	list.add(INTEGER, "3");

	list.add(RPARENS);

	list.add(LCURLY);

	list.add(RCURLY);

	IfStmt* i = new IfStmt(list,false);
	bool assert1 = (0 == list.size());

	return  (assert1)?1:0;
}

int if_test2() {

	try {
		TokenList tokens = new TokenList();

		tokens.add(IF);

		tokens.add(LPARENS);

		tokens.add(ID,"x");
		tokens.add(OPKEY,"<");
		tokens.add(INTEGER,"5");

		tokens.add(RPARENS);

		tokens.add(LCURLY);

		tokens.add(ID,"println");


		tokens.add(LPARENS);
		tokens.add(STRINGCONST,"x<5");
		tokens.add(RPARENS);

		tokens.add(SEMICOLON);

		tokens.add(RCURLY);

		IfStmt* i = new IfStmt(tokens,false);

		return 1;
	}catch (string e){
		return 0;
	}
}
