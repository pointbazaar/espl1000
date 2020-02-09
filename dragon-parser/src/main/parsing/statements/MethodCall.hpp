#ifndef METHODCALL
#define METHODCALL

#include <vector>
#include <string>

#include "../../commandline/TokenList.hpp"

class Expr;

class MethodCall {

public:
	MethodCall(TokenList tokens,bool debug);
	
	string methodName;
	vector<Expr*> arguments;
};

#endif