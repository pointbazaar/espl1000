#ifndef OP
#define OP

#include <string>

#include "../commandline/TokenList.hpp"

class Op  {

public:
	Op(TokenList tokens, bool debug);

	std::string op;
};
#endif