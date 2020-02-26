#ifndef TERM
#define TERM

struct Expr;
struct BoolConst;
struct CharConst;
struct IntConst;
struct Variable;
struct MethodCall;
struct FloatConst;
struct TokenList;

struct Term{
	//only one of these may be != NULL
	struct BoolConst* m1;
	struct IntConst* m2;
	struct CharConst* m3;
	struct MethodCall* m4;
	struct Expr* m5;
	struct Variable* m6;
	struct FloatConst* m7;
};

struct Term* makeTerm_other(struct Expr* myexpr);	
struct Term* makeTerm(struct TokenList* tokens, bool debug);

#endif