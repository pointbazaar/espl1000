#ifndef BOOLCONST
#define BOOLCONST

#include "../commandline/TokenList.hpp"

struct BoolConst{
	bool boolValue;
};

struct BoolConst* makeBoolConst(TokenList tokens, bool debug);

	
#endif