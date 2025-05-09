#pragma once

#include <stdbool.h>

#include "../ast_declare.h"

struct DeclArg {
	struct ASTNode super;

	struct Type* type;
	bool has_name;
	char* name;
};

struct MethodDecl {
	struct ASTNode super;

	struct Type* return_type;
	char* name;

	bool is_public;
	bool has_side_effects;

	uint8_t count_args;
	struct DeclArg** args;
};

struct Method {
	struct ASTNode super;

	struct MethodDecl* decl;

	struct StmtBlock* block;
};
