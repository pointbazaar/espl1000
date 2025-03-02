#pragma once

#include <stdbool.h>

#include "../ast_declare.h"

struct MAssignStmt {
	struct ASTNode super;
	struct MDirect* lhs;
	struct Expr* expr;
};
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
struct Stmt {
	struct ASTNode super;

	//only one of those will be present, 'kind' tells us
	union myptr {
		struct Call* m1;
		struct WhileStmt* m2;
		struct IfStmt* m3;
		struct RetStmt* m4;
		struct AssignStmt* m5;
		struct ForStmt* m7;
		struct SwitchStmt* m8;
		struct MAssignStmt* m9;
		struct LocalVarDeclStmt* m10;
	} ptr;
	uint8_t kind; //0-based

	//if one of these is true,
	//=> kind == 99
	bool is_continue;
	bool is_break;
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
