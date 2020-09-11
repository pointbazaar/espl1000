#ifndef CASESTMT
#define CASESTMT

#include <stdbool.h>

struct TokenList;
struct CaseStmt;

struct CaseStmt* makeCaseStmt(struct TokenList* tokens, bool debug);

#endif
