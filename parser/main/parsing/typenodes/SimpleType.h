#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <stdbool.h>

struct TokenList;
struct SimpleType;

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug);
struct SimpleType* makeSimpleType(char* typeName);

#endif
