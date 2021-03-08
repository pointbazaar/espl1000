#ifndef TYPEINFERENCE_H
#define TYPEINFERENCE_H

#include <stdbool.h>

#include "tables/symtable.h"
#include "../../ast/ast.h"

struct Type* infer_type_expr		(struct ST* st, struct Expr* expr);

struct Type* infer_type_term		(struct ST* st, struct Term* term);
struct Type* infer_type_unopterm	(struct ST* st, struct UnOpTerm* t);
struct Type* infer_type_methodcall	(struct ST* st, struct MethodCall* m);
struct Type* infer_type_variable	(struct ST* st, struct Variable* v);
struct Type* infer_type_simplevar	(struct ST* st, struct SimpleVar* v);

void print_exit(char* msg);

struct Type* unwrap_indices(struct Type* t, uint32_t count);

struct Type* unwrap(struct Type* t);

#endif
