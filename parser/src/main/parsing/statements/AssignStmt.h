#ifndef ASSIGNSTMT
#define ASSIGNSTMT

#include <stdbool.h>

struct TokenList;

#include "../../../../../ast/ast.h"

struct AssignStmt* makeAssignStmt(struct TokenList* tokens,bool debug);

void freeAssignStmt(struct AssignStmt* as);

#endif
