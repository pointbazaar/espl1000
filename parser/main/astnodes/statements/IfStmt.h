#pragma once

#include <stdbool.h>

struct TokenList;
struct IfStmt;

struct IfStmt* makeIfStmt(struct TokenList* tokens);
