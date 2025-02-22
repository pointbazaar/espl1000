#ifndef TYPEINFERENCE_H
#define TYPEINFERENCE_H

#include <stdbool.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

struct MemberAccess {
	struct Type* structType;
	struct Variable* member;
};

// @returns NULL on error
struct Type* infer_type_expr(struct ST* st, struct Expr* expr);

// @returns NULL on error
struct Type* infer_type_term(struct ST* st, struct Term* t);

// @returns NULL on error
struct Type* infer_type_unopterm(struct ST* st, struct UnOpTerm* t);

struct Type* infer_type_methodcall(struct ST* st, struct Call* m);

// @returns NULL on error
struct Type* infer_type_variable(struct ST* st, struct Variable* v);

// @returns the type after unwrapping all indices
// @returns NULL on error
struct Type* infer_type_simplevar(struct ST* st, struct SimpleVar* v);

// @returns the type
// @returns NULL on error
struct Type* infer_type_simplevar_once(struct ST* st, struct SimpleVar* v);

// @returns NULL on error
struct Type* infer_type_constvalue(struct ST* st, struct ConstValue* cv);

// @returns NULL on error
struct Type* unwrap_indices(struct Type* t, uint32_t count);

// @returns NULL on error
struct Type* unwrap(struct Type* t);

// @returns the type at the end of all member access and indexing
struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma);

// @returns the member type after one level of member access and indices
struct Type* infer_in_context_once(struct ST* st, struct MemberAccess* ma);

#endif
