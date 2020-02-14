#ifndef OP
#define OP


#include "../commandline/TokenList.hpp"

struct Op {
	char* op;
};

struct Op* makeOp(TokenList* tokens, bool debug);

#endif