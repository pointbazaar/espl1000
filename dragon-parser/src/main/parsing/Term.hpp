#ifndef TERM
#define TERM

struct Expr;
class BoolConst;
class CharConst;
class IntConst;
class Variable;
class MethodCall;
class FloatConst;

#include "../commandline/TokenList.hpp"

struct Term{
	//only one of these may be != NULL
	struct BoolConst* m1 = NULL;
	IntConst* m2 = NULL;
	CharConst* m3 = NULL;
	MethodCall* m4 = NULL;
	struct Expr* m5 = NULL;
	Variable* m6 = NULL;
	FloatConst* m7 = NULL;
};

struct Term* makeTerm(struct Expr* myexpr);	
struct Term* makeTerm(TokenList* tokens, bool debug);

#endif