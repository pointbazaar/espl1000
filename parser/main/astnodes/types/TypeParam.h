#pragma once

#include <stdbool.h>

struct TokenList;
struct TypeParam;

struct TypeParam* makeTypeParam(struct TokenList* tokens);
