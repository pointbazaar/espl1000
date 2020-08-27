#ifndef TERM
#define TERM

struct TokenList;

#include "../../../../ast/ast.h"


struct Term* makeTerm_other(struct Expr* myexpr);	
struct Term* makeTerm(struct TokenList* tokens, bool debug);

void freeTerm(struct Term* t);

#endif
