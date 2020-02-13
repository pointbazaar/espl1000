#ifndef INTCONST
#define INTCONST

#include "../commandline/TokenList.hpp"

struct IntConst {
	int number;
};

struct IntConst* makeIntConst(TokenList* tokens, bool debug);

#endif