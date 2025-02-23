#pragma once

#include <stdbool.h>

struct Id;
struct TokenList;

struct Id* makeIdentifier(struct TokenList* tokens);
