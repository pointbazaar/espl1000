#ifndef ASSIGNSTMT
#define ASSIGNSTMT

#include <stdbool.h>

struct TokenList;
struct AssignStmt;

struct AssignStmt* makeAssignStmt(struct TokenList* tokens,bool debug);

#endif
