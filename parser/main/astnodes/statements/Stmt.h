#pragma once

#include <stdbool.h>

struct TokenList;
struct Stmt;

struct Stmt* makeStmt(struct TokenList* tokens);
