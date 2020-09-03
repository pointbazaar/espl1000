#ifndef BREAKSTMT
#define BREAKSTMT

#include <stdbool.h>

struct TokenList;
struct BreakStmt;

struct BreakStmt* makeBreakStmt(struct TokenList* tokens, bool debug);

#endif
