#pragma once

#include <stdbool.h>

struct TokenList;
struct MAssignStmt;

struct MAssignStmt* makeMAssignStmt(struct TokenList* tokens);
