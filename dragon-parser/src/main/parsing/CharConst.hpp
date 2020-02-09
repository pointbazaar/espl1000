#ifndef CHARCONST
#define CHARCONST

#include "../commandline/TokenList.hpp"

class CharConst {

public:
	CharConst(TokenList tokens,bool debug);

	char content;
};

#endif