#pragma once

#include <stdbool.h>

struct TokenList;
struct StmtBlock;

struct StmtBlock* makeStmtBlock(struct TokenList* tokens);
