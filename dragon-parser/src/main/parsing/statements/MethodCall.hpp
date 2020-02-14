#ifndef METHODCALL
#define METHODCALL

class TokenList;

struct Expr;

struct MethodCall {
	
	char* methodName;

	//arguments to the subroutine call
	int count_args;
	struct Expr** args;
};

struct MethodCall* makeMethodCall(TokenList* tokens,bool debug);

#endif