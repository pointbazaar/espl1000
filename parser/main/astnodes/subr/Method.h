#pragma once

#include <stdbool.h>

struct TokenList;
struct Method;

struct Method* makeMethod(struct TokenList* tokens);
