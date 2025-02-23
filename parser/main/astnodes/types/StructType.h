#pragma once

#include <stdbool.h>

struct TokenList;
struct StructType;

// @returns NULL on error
struct StructType* makeStructType(struct TokenList* tokens);
