#ifndef INTCONST
#define INTCONST

#include "../commandline/TokenList.hpp"

class IntConst {

public:
	IntConst(TokenList* tokens, bool debug);

	int number;
};
#endif