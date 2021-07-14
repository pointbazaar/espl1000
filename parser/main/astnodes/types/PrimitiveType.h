#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

#include <stdbool.h>

struct TokenList;
struct PrimitiveType;

struct PrimitiveType* makePrimitiveType(struct TokenList* tokens);

#endif
