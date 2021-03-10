#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H

#include <stdbool.h>

struct TokenList;
struct StructType;

struct StructType* makeStructType(struct TokenList* tokens, bool debug);

#endif
