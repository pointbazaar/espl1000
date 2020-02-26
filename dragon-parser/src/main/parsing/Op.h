#ifndef OP
#define OP

struct TokenList;

struct Op {
	char* op;
};

struct Op* makeOp(struct TokenList* tokens, bool debug);

#endif