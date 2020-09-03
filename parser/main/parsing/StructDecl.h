#ifndef STRUCTDECL
#define STRUCTDECL

struct TokenList;
struct StructDecl;

struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug);

#endif
