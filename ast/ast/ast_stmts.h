#ifndef AST_STMTS_H
#define AST_STMTS_H

#include "../ast_declare.h"

struct AssignStmt {
	struct ASTNode super; 
	
	struct Type* opt_type;	//may be NULL

	struct Variable* var;
	
	struct Expr* expr;
};
struct IfStmt{
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
struct RetStmt{ 
	struct ASTNode super; 
	
	struct Expr* return_value;
};
struct Stmt {
	struct ASTNode super; 
	
	//only one of those will be present, 'kind' tells us
	union myptr {
		struct Call*       m1;
		struct WhileStmt*  m2;
		struct IfStmt*     m3;
		struct RetStmt*    m4;
		struct AssignStmt* m5;
		struct ForStmt*    m7;
		struct SwitchStmt* m8;
	} ptr;
	uint8_t kind; //0-based

	//if one of these is true,
	//=> kind == 99
	bool is_continue;
	bool is_break;
};
struct WhileStmt  {
	struct ASTNode super; 
	
	struct Expr* condition;
	struct StmtBlock* block;
};
struct ForStmt {
	struct ASTNode super; 
	
	char index_name[DEFAULT_STR_SIZE];
	struct Range* range;
	struct StmtBlock* block;
};
struct SwitchStmt{
	struct ASTNode super; 
	
	struct Expr* expr;
	
	uint32_t count_cases;
	struct CaseStmt** cases;
};
struct CaseStmt{
	struct ASTNode super; 
	//cases must have constant values
	//known at compile time
	//(to build the jump table)

	struct ConstValue* const_value;
	
	struct StmtBlock* block; //may be NULL
};
struct TryCatchStmt {
	struct ASTNode super;
	
	struct StmtBlock* try_block;
	struct StmtBlock* catch_block;
};
#endif
