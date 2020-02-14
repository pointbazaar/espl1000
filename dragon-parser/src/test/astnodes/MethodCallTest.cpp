#include "MethodCallTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/statements/MethodCall.hpp"

int methodcall_test1() {
	TokenList l = TokenList();

	l.add(ID,"main");
	l.add(LPARENS);
	l.add(RPARENS);

	struct MethodCall* call = makeMethodCall(&l,false);

	bool assert1 = (0 == l.size());
	return (assert1)?1:0;
}

int methodcall_test2() {
	TokenList list = TokenList();

	list.add(ID,"main");
	list.add(LPARENS);
	list.add(INTEGER,"4");
	list.add(RPARENS);

	struct MethodCall* call = makeMethodCall(&list,false);
	bool assert1 = (0 == list.size());

	return (assert1)?1:0;
}

int methodcall_test3() {
	TokenList list = TokenList();

	list.add(ID,"main");
	list.add(LPARENS);
	list.add(ID,"x");
	list.add(RPARENS);

	struct MethodCall* call = makeMethodCall(&list,false);
	bool assert1 = (0 == list.size());
	return (assert1)?1:0;
}

int methodcall_test_can_parse_subroutine_call() {

	TokenList tl = TokenList();
	tl.add(ID,"println");
	tl.add(LPARENS);
	tl.add(INTEGER,"1");
	tl.add(RPARENS);

	struct MethodCall* call = makeMethodCall(&tl,false);
	bool assert1 = (0 == tl.size());

	return (assert1)?1:0;
}

int methodcall_test_can_parse_subroutine_call2() {
	//println("x<5")

	TokenList tokens = TokenList();
	tokens.add(ID,"println");
	tokens.add(LPARENS);
	tokens.add(STRINGCONST,"x<5");
	tokens.add(RPARENS);

	struct MethodCall* call = makeMethodCall(&tokens,false);
	bool assert1 = (0 == tokens.size());

	return (assert1)?1:0;
}

