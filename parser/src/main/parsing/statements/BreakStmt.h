#ifndef BREAKSTMT
#define BREAKSTMT

#include <stdbool.h>

struct TokenList;
	
#include "../../../../../ast/ast.h"
struct BreakStmt* makeBreakStmt(struct TokenList* tokens, bool debug);

#endif
