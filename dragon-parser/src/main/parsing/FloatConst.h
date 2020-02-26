#ifndef FLOATCONST
#define FLOATCONST

#include <stdbool.h>

struct TokenList;

struct FloatConst{
	float value;
};

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug);

#endif