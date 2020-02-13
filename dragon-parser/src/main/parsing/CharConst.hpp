#ifndef CHARCONST
#define CHARCONST

#include "../commandline/TokenList.hpp"

struct CharConst {
	char content;
};

struct CharConst* makeCharConst(TokenList tokens,bool debug);

#endif