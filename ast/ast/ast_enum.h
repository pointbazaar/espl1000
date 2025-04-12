#pragma once

#include "../ast_declare.h"

struct EnumDecl {
	struct ASTNode super;

	// must be all uppercase (underscores are ok)
	char* name;

	struct EnumMember** members;
	uint16_t count_members;
};

struct EnumMember {
	struct ASTNode super;

	char* name;
	struct ConstValue* value;
};
