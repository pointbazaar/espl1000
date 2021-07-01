#ifndef AST_STRUCT_H
#define AST_STRUCT_H

#include "../ast_declare.h"

struct StructDecl{
	struct ASTNode super; 
	
	struct SimpleType* type; //the type(and name)
	
	struct StructMember** members;
	uint16_t count_members;
};

struct StructMember{
	struct ASTNode super; 
	
	struct Type* type;
	char name[DEFAULT_STR_SIZE];
};

#endif
