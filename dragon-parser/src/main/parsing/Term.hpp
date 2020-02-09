#ifndef TERM
#define TERM

class Expr;
class BoolConst;
class CharConst;
class IntConst;
class Variable;
class MethodCall;

#include "../commandline/TokenList.hpp"

class Term {

public:

	Term(Expr* myexpr);
	
	Term(TokenList tokens, bool debug);

	//only one of these may be != NULL
	BoolConst* m1 = NULL;
	IntConst* m2 = NULL;
	CharConst* m3 = NULL;
	MethodCall* m4 = NULL;
	Expr* m5 = NULL;
	Variable* m6 = NULL;

};

#endif