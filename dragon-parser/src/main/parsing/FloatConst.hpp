#ifndef FLOATCONST
#define FLOATCONST

class TokenList;

class FloatConst{

public:
	float value;

	FloatConst(TokenList* tokens, bool debug);
};

#endif