#ifndef METHODCALL
#define METHODCALL

#include "../../commandline/TokenList.hpp"

class Expr;

struct MethodCall {
	
	char* methodName;

	//arguments to the subroutine call
	int count_args;
	struct Expr** args;
};

struct MethodCall* makeMethodCall(TokenList tokens,bool debug);

#endif