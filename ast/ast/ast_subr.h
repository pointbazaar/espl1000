#ifndef AST_SUBR_H
#define AST_SUBR_H

#include "../ast_declare.h"

struct DeclArg  {
	struct Type* type;
	bool has_name;
	char name[DEFAULT_STR_SIZE];
};

struct Method {
	struct Type* returnType;
	char name[DEFAULT_STR_SIZE];
	
	bool isPublic;
	bool hasSideEffects;

	uint8_t count_args;
	struct DeclArg** args;

	struct StmtBlock* block;
};

#endif
