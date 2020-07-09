#ifndef STMT
#define STMT

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct Stmt* makeStmt(struct TokenList* tokens, bool debug);

#endif
