#ifndef HEXCONST_H
#define HEXCONST_H

#include <stdbool.h>

struct HexConst;
struct TokenList;

struct HexConst* makeHexConst(struct TokenList* tokens);

#endif
