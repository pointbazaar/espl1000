#include "IfStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/statements/IfStmt.hpp"

int if_test1() {
	TokenList* list = new TokenList();
	list->add(IF);

	//list->add(LPARENS);

	list->add(INTEGER, "5");
	list->add(OPKEY, "<");
	list->add(INTEGER, "3");

	//list->add(RPARENS);

	list->add(LCURLY);

	list->add(RCURLY);

	IfStmt* i = new IfStmt(*list,false);
	bool assert1 = (0 == list->size());

	return  (assert1)?1:0;
}

int if_test2() {

	try {
		TokenList* l = new TokenList();

		l->add(IF);

		//l->add(LPARENS);

		l->add(ID,"x");
		l->add(OPKEY,"<");
		l->add(INTEGER,"5");

		//l->add(RPARENS);

		l->add(LCURLY);

		l->add(ID,"println");


		l->add(LPARENS);
		l->add(STRINGCONST,"x<5");
		l->add(RPARENS);

		l->add(SEMICOLON);

		l->add(RCURLY);

		IfStmt* i = new IfStmt(*l,true);

		return 1;
	}catch (string e){
		return 0;
	}
}
