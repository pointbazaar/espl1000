#ifndef TYPEINFERENCE
#define TYPEINFERENCE

struct ST;
struct Expr;

char* inferTypeExpr(struct ST* st, struct Expr* expr);

#endif
