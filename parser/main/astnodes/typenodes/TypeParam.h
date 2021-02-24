#ifndef TYPEPARAM
#define TYPEPARAM

#include <stdbool.h>

struct TokenList;
struct TypeParam;

struct TypeParam* makeTypeParam(struct TokenList* tokens, bool debug);

#endif
