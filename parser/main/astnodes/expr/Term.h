#ifndef TERM
#define TERM

struct TokenList;
struct Term;
struct Expr;

struct Term* makeTerm_other(struct Expr* myexpr);
struct Term* makeTerm(struct TokenList* tokens);

#endif
