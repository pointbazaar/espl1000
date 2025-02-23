#pragma once

#include <stdbool.h>

struct TokenList;
struct Type;
struct SubrType;

struct SubrType* makeSubrType(struct TokenList* tokens);
