#include "WhileStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/WhileStmt.h"

int whilestmt_test1() {
	try {
		TokenList list = TokenList();
		list.add(WHILE);

		list.add(LPARENS);

		list.add(INTEGER,"5");
		list.add(OPKEY,"<");
		list.add(INTEGER,"3");

		list.add(RPARENS);

		list.add(LCURLY);

		list.add(RCURLY);

		struct WhileStmt* whileStatement = makeWhileStmt(&list,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

