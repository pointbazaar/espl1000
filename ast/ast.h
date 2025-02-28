#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

#include "ast_declare.h"

#include "ast/ast_types.h"
#include "ast/ast_stmts.h"
#include "ast/ast_const.h"
#include "ast/ast_struct.h"
#include "ast/ast_var.h"
#include "ast/ast_expr.h"
#include "ast/ast_subr.h"

/* To quickly allocate Instances. Instead of
 * x = malloc(sizeof(struct IntConst));
 * we have:
 * x = make(IntConst);
 * which is way shorter
 */
#define make(X) calloc(1, sizeof(struct X))

struct AST {
	struct Namespace** namespaces;
	uint16_t count_namespaces;
};

struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char* src_path; //the .dg filename
	char* token_path; //the .dg.tokens filename

	char* name;

	uint16_t count_includes;
	char** includes;

	//structs must be declared before the subroutines
	struct StructDecl** structs;
	uint16_t count_structs;
	size_t capacity_structs;

	struct Method** methods;
	uint16_t count_methods;
	size_t capacity_methods;
};

struct Range {
	struct ASTNode super;
	struct Expr* start;
	struct Expr* end;
};

struct Id {
	struct ASTNode super;
	char* identifier;
};

struct StmtBlock {
	struct ASTNode super;
	uint16_t count;
	struct Stmt** stmts;
};
