#ifndef WHILESTMT
#define WHILESTMT

#include <stdbool.h>

struct TokenList;
struct WhileStmt;

struct WhileStmt* makeWhileStmt(struct TokenList* tokens,bool debug);

#endif
