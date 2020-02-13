#ifndef FLOATCONST
#define FLOATCONST

class TokenList;

struct FloatConst{
	float value;
};

struct FloatConst* makeFloatConst(TokenList* tokens, bool debug);

#endif