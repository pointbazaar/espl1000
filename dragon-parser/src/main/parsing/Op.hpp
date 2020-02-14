#ifndef OP
#define OP

class TokenList;

struct Op {
	char* op;
};

struct Op* makeOp(TokenList* tokens, bool debug);

#endif