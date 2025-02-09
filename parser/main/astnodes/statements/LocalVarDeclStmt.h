#pragma once

#include <stdbool.h>

struct TokenList;
struct LocalVarDeclStmt;

struct LocalVarDeclStmt* makeLocalVarDeclStmt(struct TokenList* tokens);
