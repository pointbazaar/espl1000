#ifndef SIMPLEVAR
#define SIMPLEVAR

class TokenList;
struct Expr;

struct SimpleVar {

	char* name;
	//may be NULL
	struct Expr* indexOptional = NULL;
};

struct SimpleVar* makeSimpleVar(TokenList* tokens, bool debug);

#endif