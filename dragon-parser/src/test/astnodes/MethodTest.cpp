#include "MethodTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/Method.hpp"

int method_test_can_parse_method_with_arguments() {

	TokenList l = new TokenList(); //ParserPhases.makeTokenList("fn main (String hello)~>PInt { main(); }", false);

	l.add(FN);
	l.add(ID,"main");
	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"String");
	l.add(ID,"hello");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(LCURLY);
		l.add(ID,"main");
		l.add(LPARENS);
		l.add(RPARENS);
		l.add(SEMICOLON);

	l.add(RCURLY);

	Method* m = new Method(l, false);

	return  m->arguments.size();
}

int method_test_can_parse_subroutine() {

	try {
		TokenList l = new TokenList();

		l.add(FN);
		l.add(ID,"main");
		l.add(LPARENS);
		l.add(RPARENS);
		l.add(ARROW);
		l.add(TYPEIDENTIFIER,"PInt")
		l.add(LCURLY);
			l.add(ID,"println");
			l.add(LPARENS);
			l.add(RPARENS);
			l.add(SEMICOLON);

			l.add(RETURN);
			l.add(INTEGER,0);
			
		l.add(RCURLY);

		Method* m = new Method(l, false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int method_test_can_parse_method_without_arguments() {

	TokenList l = new TokenList();

	l.add(FN);
	l.add(ID,"main");
	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"String");
	l.add(ID,"hello");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(LCURLY);
		l.add(ID,"main");
		l.add(LPARENS);
		l.add(RPARENS);
		l.add(SEMICOLON);

	l.add(RCURLY);

	Method* m = new Method(l, false);
	bool assert1 = (0 == m->arguments.size());

	return (assert1)?1:0;
}
