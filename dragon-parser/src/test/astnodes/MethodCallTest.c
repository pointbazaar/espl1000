#include "MethodCallTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/MethodCall.h"

#include <stdbool.h>

int methodcall_test1(bool debug) {
	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(ID,"main"));
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken(RPARENS));

	struct MethodCall* call = makeMethodCall(l,false);

	bool assert1 = (0 == list_size(l));
	bool assert2 = call != NULL;
	return (assert1 && assert2)?1:0;
}

int methodcall_test2(bool debug) {
	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"main"));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken2(INTEGER,"4"));
	list_add(list, makeToken(RPARENS));

	struct MethodCall* call = makeMethodCall(list,false);

	bool assert1 = (0 == list_size(list));
	bool assert2 = call != NULL;

	return (assert1 && assert2)?1:0;
}

int methodcall_test3(bool debug) {
	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"main"));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken(RPARENS));

	struct MethodCall* call = makeMethodCall(list,false);

	bool assert1 = (0 == list_size(list));
	bool assert2 = call != NULL;

	return (assert1 && assert2)?1:0;
}

int methodcall_test_can_parse_subroutine_call(bool debug) {

	struct TokenList* tl = makeTokenList();
	list_add(tl, makeToken2(ID,"println"));
	list_add(tl, makeToken(LPARENS));
	list_add(tl, makeToken2(INTEGER,"1"));
	list_add(tl, makeToken(RPARENS));

	struct MethodCall* call = makeMethodCall(tl,false);

	bool assert1 = (0 == list_size(tl));
	bool assert2 = call != NULL;

	return (assert1 && assert2)?1:0;
}

int methodcall_test_can_parse_subroutine_call2(bool debug) {
	//println("x<5")

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"println"));
	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken2(STRINGCONST,"x<5"));
	list_add(tokens, makeToken(RPARENS));

	struct MethodCall* call = makeMethodCall(tokens,false);

	bool assert1 = (0 == list_size(tokens));
	bool assert2 = call != NULL;

	return (assert1 && assert2)?1:0;
}

