#ifndef INTCONST
#define INTCONST

class TokenList;

struct IntConst {
	int number;
};

struct IntConst* makeIntConst(TokenList* tokens, bool debug);

#endif