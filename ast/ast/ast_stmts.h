#pragma once

#include <stdbool.h>

#include "../ast_declare.h"

struct AssignStmt {
	struct ASTNode super;

	struct Type* opt_type; //may be NULL

	struct LValue* lvalue;

	struct Expr* expr;
};
struct IfStmt {
	struct ASTNode super;

	struct Expr* condition;
	struct StmtBlock* block;
	struct StmtBlock* else_block; //may be NULL
};
struct Call {
	struct ASTNode super;

	struct Variable* callable;

	uint8_t count_args;
	struct Expr** args;
};
struct RetStmt {
	struct ASTNode super;

	struct Expr* return_value;
};
struct LocalVarDeclStmt {
	struct ASTNode super;

	struct Type* type;
	char* name;
};

enum STMT_KIND {
	STMT_KIND_CALL,
	STMT_KIND_WHILE,
	STMT_KIND_IF,
	STMT_KIND_RETURN,
	STMT_KIND_ASSIGN,
	STMT_KIND_FOR,

	STMT_KIND_LOCAL_VAR_DECL,

	STMT_KIND_BREAK,
	STMT_KIND_CONTINUE,
};

struct Stmt {
	struct ASTNode super;

	//only one of those will be present, 'kind' tells us
	union myptr {
		struct Call* call;
		struct WhileStmt* while_stmt;
		struct IfStmt* if_stmt;
		struct RetStmt* return_stmt;
		struct AssignStmt* assign_stmt;
		struct ForStmt* for_stmt;
		struct LocalVarDeclStmt* local_var_decl_stmt;
	} ptr;
	enum STMT_KIND kind;
};
struct WhileStmt {
	struct ASTNode super;

	struct Expr* condition;
	struct StmtBlock* block;
};
struct ForStmt {
	struct ASTNode super;

	char* index_name;
	struct Range* range;
	struct StmtBlock* block;
};
