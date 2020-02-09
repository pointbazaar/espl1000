#ifndef BOOLCONST
#define BOOLCONST

#include "../commandline/TokenList.hpp"

class BoolConst {

public:
	BoolConst(TokenList tokens, bool debug);

	bool boolValue;
};

#endif