#pragma once

#include <stdbool.h>

struct DeclArg;
struct TokenList;

struct DeclArg* makeDeclArg(struct TokenList* tokens);
