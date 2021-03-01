#ifndef STMT
#define STMT

#include <stdbool.h>

struct TokenList;
struct Stmt;

struct Stmt* makeStmt(struct TokenList* tokens, bool debug);

#endif
