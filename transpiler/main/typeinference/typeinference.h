#ifndef TYPEINFERENCE
#define TYPEINFERENCE

#include <stdbool.h>

#include "tables/symtable.h"
#include "../../ast/ast.h"

struct Type* inferTypeExpr			(struct ST* st, struct Expr* expr, bool debug);

//----------------------------------------

struct Type* inferTypeTerm			(struct ST* st, struct Term* term, bool debug);
struct Type* inferTypeUnOpTerm		(struct ST* st, struct UnOpTerm* t, bool debug);
struct Type* inferTypeMethodCall	(struct ST* st, struct MethodCall* m, bool debug);
struct Type* inferTypeVariable		(struct ST* st, struct Variable* v, bool debug);
struct Type* inferTypeSimpleVar		(struct ST* st, struct SimpleVar* v, bool debug);

#endif
