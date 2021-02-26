#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

#include "ast_declare.h"

#include "ast_types.h"
#include "ast_stmts.h"
#include "ast_const.h"
#include "ast_struct.h"
#include "ast_var.h"
#include "ast_expr.h"

struct AST_Whole_Program  {
	struct Namespace** namespaces;
	uint16_t count_namespaces;
};
struct DeclArg  {
	struct Type* type;
	bool has_name;
	char name[DEFAULT_STR_SIZE];
};
struct Range { struct Expr* start; struct Expr* end; };
//--------------
struct Identifier  { char identifier[DEFAULT_STR_SIZE]; };

struct Method {
	struct Type* returnType;
	char name[DEFAULT_STR_SIZE];
	
	bool isPublic;
	bool hasSideEffects;

	uint8_t count_args;
	struct DeclArg** args;

	struct StmtBlock* block;
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
struct StmtBlock {
	uint16_t count;
	struct Stmt** stmts;
};

#endif
