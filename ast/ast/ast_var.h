#ifndef AST_VAR_H
#define AST_VAR_H

#include "../ast_declare.h"

struct Variable {
	struct ASTNode super; 
	
	struct SimpleVar* simpleVar;
	
	struct Variable* memberAccess; //may be NULL
};
struct SimpleVar {
	struct ASTNode super; 
	
	char name[DEFAULT_STR_SIZE];
	
	uint8_t count_indices;
	struct Expr** indices;
};

#endif
