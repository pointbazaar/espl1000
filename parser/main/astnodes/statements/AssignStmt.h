#pragma once

#include <stdbool.h>

struct TokenList;
struct AssignStmt;

struct AssignStmt* makeAssignStmt(struct TokenList* tokens);
