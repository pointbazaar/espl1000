#ifndef IFSTMT
#define IFSTMT

#include <stdbool.h>

struct TokenList;
	
#include "../../../../../ast/ast.h"
struct IfStmt* makeIfStmt(struct TokenList* tokens,bool debug);

#endif
