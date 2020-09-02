#ifndef STMTBLOCK
#define STMTBLOCK

struct TokenList;
#include "../../../../ast/ast.h"

struct StmtBlock* makeStmtBlock(struct TokenList* tokens, bool debug);

#endif
