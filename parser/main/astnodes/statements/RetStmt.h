#ifndef RETURNSTMT
#define RETURNSTMT

#include <stdbool.h>

struct TokenList;
struct RetStmt;

struct RetStmt* makeRetStmt(struct TokenList* tokens);

#endif
