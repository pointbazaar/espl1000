#pragma once

#include <stdbool.h>

struct TokenList;
struct Type;
struct ArrayType;

struct ArrayType* makeArrayType(struct Type* element_type);
struct ArrayType* makeArrayType2(struct TokenList* tokens);
