#pragma once

#include <stdbool.h>

struct TokenList;
struct SimpleType;

struct SimpleType* makeSimpleType(struct TokenList* tokens);
