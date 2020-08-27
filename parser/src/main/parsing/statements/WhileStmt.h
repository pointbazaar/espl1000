#ifndef WHILESTMT
#define WHILESTMT

#include <stdbool.h>

struct TokenList;
	
#include "../../../../../ast/ast.h"
struct WhileStmt* makeWhileStmt(struct TokenList* tokens,bool debug);

void freeWhileStmt(struct WhileStmt* ws);

#endif
