#pragma once

#include "../ast_declare.h"

struct StructDecl {
	struct ASTNode super;

	// TODO: should be 'StructType' ?
	struct SimpleType* type; //the type(and name)

	struct StructMember** members;
	uint16_t count_members;
};

struct StructMember {
	struct ASTNode super;

	struct Type* type;
	char* name;
};
