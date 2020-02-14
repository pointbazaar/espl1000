#ifndef BOOLCONST
#define BOOLCONST

class TokenList;

struct BoolConst{
	bool boolValue;
};

struct BoolConst* makeBoolConst(TokenList* tokens, bool debug);

	
#endif