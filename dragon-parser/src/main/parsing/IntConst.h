#ifndef INTCONST
#define INTCONST

#include <stdbool.h>

struct TokenList;

struct IntConst {
	int value;
};

struct IntConst* makeIntConst(struct TokenList* tokens, bool debug);

#endif