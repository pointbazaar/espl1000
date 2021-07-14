#ifndef AST_SUBR_H
#define AST_SUBR_H

#include "../ast_declare.h"

struct DeclArg  {
	struct ASTNode super; 
	
	struct Type* type;
	bool has_name;
	char name[DEFAULT_STR_SIZE];
};

struct Method {
	struct ASTNode super; 
	
	struct Type* return_type;
	char name[DEFAULT_STR_SIZE];
	
	bool is_public;
	bool has_side_effects;
	bool throws;

	uint8_t count_args;
	struct DeclArg** args;

	struct StmtBlock* block;
};

#endif
