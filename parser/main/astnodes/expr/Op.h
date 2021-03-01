#ifndef OP
#define OP

struct TokenList;
struct Op;

struct Op* makeOp(struct TokenList* tokens, bool debug);

#endif
