#include "WhileStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/WhileStmt.h"

#include <stdbool.h>

int whilestmt_test1() {
	
	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(WHILE) );

	list_add(list, makeToken(LPARENS) );

	list_add(list, makeToken2(INTEGER,"5") );
	list_add(list, makeToken2(OPKEY,"<") );
	list_add(list, makeToken2(INTEGER,"3") );

	list_add(list, makeToken(RPARENS) );

	list_add(list, makeToken(LCURLY) );

	list_add(list, makeToken(RCURLY) );

	struct WhileStmt* ws = makeWhileStmt(list,false);

	return (ws!=NULL)?1:0;
}

