#ifndef RETURNSTMT
#define RETURNSTMT

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct RetStmt* makeRetStmt(struct TokenList* list,bool debug);

#endif
