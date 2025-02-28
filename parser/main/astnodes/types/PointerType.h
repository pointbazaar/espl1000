#pragma once

#include <stdbool.h>

struct TokenList;
struct Type;
struct PointerType;

struct PointerType* makePointerType(struct Type* element_type);
struct PointerType* makePointerType2(struct TokenList* tokens);
