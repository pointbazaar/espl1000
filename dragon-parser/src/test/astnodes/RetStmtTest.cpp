#include "RetStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/statements/RetStmt.hpp"

int retstmt_test1() {
	try {
		TokenList list = TokenList();
		list.add(RETURN);

		list.add(LPARENS);

		list.add(Token(OPKEY,"-"));
		list.add(Token(INTEGER,"5"));

		list.add(RPARENS);

		list.add(SEMICOLON);

		RetStmt* r = new RetStmt(list,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int retstmt_test2(){
	try {
		TokenList list = TokenList();
		list.add(RETURN);

		list.add(LPARENS);

		list.add(Token(OPKEY,"-"));

		list.add(Token(INTEGER,"5"));

		list.add(RPARENS);

		list.add(Token(OPKEY,"*"));

		list.add(Token(ID,"n"));

		list.add(SEMICOLON);

		RetStmt* r = new RetStmt(list,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int retstmt_test3() {

	try {
		TokenList list = TokenList();

		list.add(Token(RETURN));

		list.add(Token(ID,"arr"));

		list.add(Token(LBRACKET));
		list.add(Token(INTEGER,"0"));

		list.add(Token(RBRACKET));

		list.add(Token(SEMICOLON));

		RetStmt* r = new RetStmt(list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}
