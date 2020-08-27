#ifndef LOOPSTMT
#define LOOPSTMT

#include <stdbool.h>

struct TokenList;
	
#include "../../../../../ast/ast.h"
struct LoopStmt* makeLoopStmt(struct TokenList* tokens, bool debug);

void freeLoopStmt(struct LoopStmt* is);

#endif
