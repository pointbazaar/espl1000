#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H

#include <stdbool.h>

struct TokenList;
struct StructType;

// @returns NULL on error
struct StructType* makeStructType(struct TokenList* tokens);

#endif
