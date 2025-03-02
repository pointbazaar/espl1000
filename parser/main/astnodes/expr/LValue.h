#pragma once

#include <stdbool.h>

struct TokenList;
struct LValue;
struct Term;
struct Variable;

struct LValue* makeLValue(struct Variable* var);
struct LValue* makeLValue2(struct TokenList* tokens);
