#pragma once

#include <stdbool.h>

struct TokenList;
struct ForStmt;

struct ForStmt* makeForStmt(struct TokenList* tokens);
