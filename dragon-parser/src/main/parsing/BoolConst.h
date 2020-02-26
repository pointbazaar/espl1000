#ifndef BOOLCONST
#define BOOLCONST

#include <stdbool.h>

struct TokenList;

struct BoolConst{
	bool value;
};

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug);

	
#endif