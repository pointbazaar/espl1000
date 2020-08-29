#ifndef TYPEINFERENCE
#define TYPEINFERENCE

#include "localvarsymtable.h"

struct Expr;

char* inferTypeExpr(struct LVST* lsvt, struct Expr* expr);

#endif
