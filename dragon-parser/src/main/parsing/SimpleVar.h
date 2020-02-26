#ifndef SIMPLEVAR
#define SIMPLEVAR

struct TokenList;
struct Expr;

struct SimpleVar {

	char* name;
	//may be NULL
	struct Expr* indexOptional;
};

struct SimpleVar* makeSimpleVar(struct TokenList* tokens, bool debug);

#endif