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
#include "ast/ast_lambda.h"

/* To quickly allocate Instances. Instead of 
 * x = malloc(sizeof(struct IntConst));
 * we have:
 * x = make(IntConst);
 * which is way shorter
 */
#define make(X) malloc(sizeof(struct X))

struct AST {
	struct Namespace** namespaces;
	uint16_t count_namespaces;
};

struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char* src_path; 	//the .dg filename
	char* token_path; 	//the .dg.tokens filename
	char* ast_path; 	//the .ast filename
	
	char name[DEFAULT_STR_SIZE];

	//passthrough-includes must be declared before externc statements
	uint16_t count_includes;
	char** includes;

	//externc subroutines must be declared before structs
	struct ExternC** externc;
	uint16_t count_externc;
	size_t capacity_externc;

	//structs must be declared before the subroutines
	struct StructDecl** structs;
	uint16_t count_structs;
	size_t capacity_structs;

	struct Method** methods;
	uint16_t count_methods;
	size_t capacity_methods;
};

struct Range { struct ASTNode super; struct Expr* start; struct Expr* end; };

struct Id  { struct ASTNode super; char identifier[DEFAULT_STR_SIZE]; };

struct StmtBlock {
	struct ASTNode super; 
	uint16_t count;
	struct Stmt** stmts;
};

#endif
