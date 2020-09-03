#ifndef IFSTMT
#define IFSTMT

#include <stdbool.h>

struct TokenList;
struct IfStmt;

struct IfStmt* makeIfStmt(struct TokenList* tokens,bool debug);

#endif
