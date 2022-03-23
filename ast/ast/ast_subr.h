#ifndef AST_SUBR_H
#define AST_SUBR_H

#include "../ast_declare.h"

struct DeclArg  {
	struct ASTNode super; 
	
	struct Type* type;
	bool has_name;
	char name[DEFAULT_STR_SIZE];
};

struct ExternC {
	struct MethodDecl* subr_decl; //may be NULL
	struct StructDecl* struct_decl; //may be NULL
	char name_in_c[DEFAULT_STR_SIZE];
};

struct MethodDecl{
	struct ASTNode super;

	struct Type* return_type;
	char name[DEFAULT_STR_SIZE];

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

#endif
