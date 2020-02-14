#ifndef TERM
#define TERM

struct Expr;
struct BoolConst;
class CharConst;
class IntConst;
struct Variable;
class MethodCall;
class FloatConst;

#include "../commandline/TokenList.hpp"

struct Term{
	//only one of these may be != NULL
	struct BoolConst* m1 = NULL;
	struct IntConst* m2 = NULL;
	struct CharConst* m3 = NULL;
	struct MethodCall* m4 = NULL;
	struct Expr* m5 = NULL;
	struct Variable* m6 = NULL;
	struct FloatConst* m7 = NULL;
};

struct Term* makeTerm(struct Expr* myexpr);	
struct Term* makeTerm(TokenList* tokens, bool debug);

#endif