#pragma once

#include "../ast_declare.h"

struct Variable {
	struct ASTNode super;

	struct SimpleVar* simple_var;
	struct Variable* member_access; //may be NULL
};
struct SimpleVar {
	struct ASTNode super;

	char* name;

	uint8_t count_indices;
	struct Expr** indices;
};
