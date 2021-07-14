#ifndef LOOPSTMT
#define LOOPSTMT

#include <stdbool.h>

struct TokenList;
struct LoopStmt;

struct LoopStmt* makeLoopStmt(struct TokenList* tokens);

#endif
