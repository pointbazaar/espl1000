#pragma once

#include <stdbool.h>

struct TokenList;
struct Deref;
struct Term;

struct Deref* makeDeref(struct Term* term);
struct Deref* makeDeref2(struct TokenList* tokens);
