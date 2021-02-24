#ifndef BINCONST_H
#define BINCONST_H

#include <stdbool.h>

struct BinConst;
struct TokenList;

struct BinConst* makeBinConst(struct TokenList* tokens, bool debug);

#endif
