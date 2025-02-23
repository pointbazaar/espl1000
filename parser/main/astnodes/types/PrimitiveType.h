#pragma once

#include <stdbool.h>

struct TokenList;
struct PrimitiveType;

struct PrimitiveType* makePrimitiveType(struct TokenList* tokens);
