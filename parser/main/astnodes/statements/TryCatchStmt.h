#ifndef TRYCATCH_STMT_H
#define TRYCATCH_STMT_H

#include <stdbool.h>

struct TokenList;
struct TryCatchStmt;

struct TryCatchStmt* makeTryCatchStmt(struct TokenList* tokens, bool debug);

#endif
