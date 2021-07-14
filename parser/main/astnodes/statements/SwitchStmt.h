#ifndef SWITCHSTMT
#define SWITCHSTMT

#include <stdbool.h>

struct TokenList;
struct SwitchStmt;

struct SwitchStmt* makeSwitchStmt(struct TokenList* tokens);

#endif
