#include "LoopStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/statements/LoopStmt.hpp"

int loop_test1()  {
	try {
		TokenList list = new TokenList();
		list.add(LOOP);
		list.add(INTEGER,5);
		list.add(LCURLY);

		list.add(RCURLY);

		LoopStmt* loop = new LoopStmt(list);

		return 1;
	}catch (string e){
		return 0;
	}
}
