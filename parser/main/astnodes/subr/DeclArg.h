#ifndef DECLARG
#define DECLARG

#include <stdbool.h>

struct DeclArg;
struct TokenList;

struct DeclArg* makeDeclArg(struct TokenList* tokens);

#endif
