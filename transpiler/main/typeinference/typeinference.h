#ifndef TYPEINFERENCE
#define TYPEINFERENCE

#include <stdbool.h>

#include "tables/symtable.h"
#include "../../ast/ast.h"

struct Type* inferTypeExpr(struct ST* st, struct Expr* expr, bool debug);

#endif
