#pragma once

#include <stdbool.h>

struct ConstValue;
struct TokenList;

struct ConstValue* makeConstValue(struct TokenList* tokens);
