#ifndef TYPEINFERENCE
#define TYPEINFERENCE

#include "tables/symtable.h"
#include "../../ast/ast.h"

struct Type* inferTypeExpr(struct ST* st, struct Expr* expr);
char* typeToStr(struct Type* t);
#endif
