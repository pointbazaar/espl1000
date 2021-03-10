#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <stdbool.h>

struct TokenList;
struct SimpleType;

struct SimpleType* makeSimpleType(struct TokenList* tokens, bool debug);

#endif
