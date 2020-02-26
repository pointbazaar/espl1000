#ifndef METHODCALL
#define METHODCALL

#include <stdbool.h>

struct TokenList;
struct Expr;

struct MethodCall {
	
	char* methodName;

	//arguments to the subroutine call
	int count_args;
	struct Expr** args;
};

struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug);

#endif