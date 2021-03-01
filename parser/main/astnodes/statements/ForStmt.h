#ifndef FORSTMT
#define FORSTMT

#include <stdbool.h>

struct TokenList;
struct ForStmt;

struct ForStmt* makeForStmt(struct TokenList* tokens,bool debug);

#endif
