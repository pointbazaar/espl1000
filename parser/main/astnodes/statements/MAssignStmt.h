#ifndef SMALLDRAGON_TOPLEVEL_MASSIGNSTMT_H
#define SMALLDRAGON_TOPLEVEL_MASSIGNSTMT_H

#include <stdbool.h>

struct TokenList;
struct MAssignStmt;

struct MAssignStmt* makeMAssignStmt(struct TokenList* tokens);

#endif
