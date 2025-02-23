#pragma once

#include <stdbool.h>

struct TokenList;
struct WhileStmt;

struct WhileStmt* makeWhileStmt(struct TokenList* tokens);
