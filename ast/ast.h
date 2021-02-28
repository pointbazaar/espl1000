#ifndef AST_H
#define AST_H

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

struct AST_Whole_Program  {
	struct Namespace** namespaces;
	uint16_t count_namespaces;
};

struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char srcPath[DEFAULT_STR_SIZE];
	char name[DEFAULT_STR_SIZE];

	//structs must be declared before the subroutines
	struct Method** methods;
	uint16_t count_methods;
	size_t capacity_methods;

	struct StructDecl** structs;
	uint16_t count_structs;
	size_t capacity_structs;
};

struct Range { struct Expr* start; struct Expr* end; };

struct Identifier  { char identifier[DEFAULT_STR_SIZE]; };

struct StmtBlock {
	uint16_t count;
	struct Stmt** stmts;
};

#endif
