#pragma once

#include <stdbool.h>

struct TokenList;
struct UnOpTerm;

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens);
