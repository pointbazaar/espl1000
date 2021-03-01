#ifndef AST_VAR_H
#define AST_VAR_H

#include "../ast_declare.h"

struct Variable {
	
	struct SimpleVar* simpleVar;
	
	struct Variable* memberAccess; //may be NULL
};
struct SimpleVar {
	
	char name[DEFAULT_STR_SIZE];
	
	uint8_t count_indices;
	struct Expr** indices;
};

#endif
