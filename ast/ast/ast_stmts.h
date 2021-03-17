#ifndef AST_STMTS_H
#define AST_STMTS_H

#include "../ast_declare.h"

#define ASSIGNOP_LENGTH 4

struct AssignStmt {
	struct Type* optType;	//may be NULL

	struct Variable* var;
	
	char assign_op[ASSIGNOP_LENGTH]; //[=,+=,-=, ...]
	
	struct Expr* expr;
};
struct IfStmt{
	struct Expr* condition;
	struct StmtBlock* block;
	struct StmtBlock* elseBlock; //may be NULL
};
struct MethodCall {
	char methodName[DEFAULT_STR_SIZE];

	uint8_t count_args;
	struct Expr** args;
};
struct RetStmt{ struct Expr* returnValue; };
struct Stmt {
	//only one of those will be present,
	//'kind' tells us which
	union myptr {
		struct LoopStmt* m0;
		struct MethodCall* m1;
		struct WhileStmt* m2;
		struct IfStmt* m3;
		struct RetStmt* m4;
		struct AssignStmt* m5;
		struct ForStmt* m7;
		struct SwitchStmt* m8;
	} ptr;
	uint8_t kind; //0-based

	//if one of these is true,
	//=> kind == 99
	bool isContinue;
	bool isBreak;
};
struct WhileStmt  {
	struct Expr* condition;
	struct StmtBlock* block;
};
struct LoopStmt {
	struct Expr* count;
	struct StmtBlock* block;
};
struct ForStmt {
	char indexName[DEFAULT_STR_SIZE];
	struct Range* range;
	struct StmtBlock* block;
};
struct SwitchStmt{
	struct Expr* expr;
	
	uint32_t count_cases;
	struct CaseStmt** cases;
};
struct CaseStmt{
	//cases must have constant values
	//known at compile time
	//(to build the jump table)
	
	//kind says which pointer is present
	//from the alternatives
	uint8_t kind; // \in {0,1,2}
	union my_ptr {
		struct BoolConst* m1;
		struct CharConst* m2;
		struct IntConst* m3;
	} ptr;
	
	struct StmtBlock* block; //may be NULL
};

#endif
