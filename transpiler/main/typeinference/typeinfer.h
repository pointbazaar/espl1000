#ifndef TYPEINFERENCE_H
#define TYPEINFERENCE_H

#include <stdbool.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

struct Type* infer_type_expr		(char* filename, struct ST* st, struct Expr* expr);

struct Type* infer_type_term		(char* filename, struct ST* st, struct Term* term);
struct Type* infer_type_unopterm	(char* filename, struct ST* st, struct UnOpTerm* t);
struct Type* infer_type_methodcall	(                struct ST* st, struct Call* m);
struct Type* infer_type_variable	(char* filename, struct ST* st, struct Variable* v);
struct Type* infer_type_simplevar	(char* filename, struct ST* st, struct SimpleVar* v);

void print_exit(char* filename, struct ASTNode* node, char* msg);

struct Type* unwrap_indices(char* filename, struct Type* t, uint32_t count);

struct Type* unwrap(char* filename, struct Type* t);

#endif
