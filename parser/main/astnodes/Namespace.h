#ifndef NAMESPACE
#define NAMESPACE

struct TokenList;
struct Namespace;

struct Namespace* makeNamespace(struct TokenList* tokens, char* name);

#endif
