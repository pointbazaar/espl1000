#ifndef AST_STRUCT_H
#define AST_STRUCT_H

#include "../ast_declare.h"

struct StructDecl{
	
	struct SimpleType* type; //the type(and name)
	
	struct StructMember** members;
	uint16_t count_members;
};

struct StructMember{
	
	struct Type* type;
	char name[DEFAULT_STR_SIZE];
};

#endif
