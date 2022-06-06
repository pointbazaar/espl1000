#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "../ast_declare.h"

struct Expr {
	struct ASTNode super; 
	struct UnOpTerm* term1;
	struct Op* op;          //may be NULL
	struct UnOpTerm* term2; //may be NULL
};
struct Op { 
	struct ASTNode super; 
	char op[6]; 
	
	bool is_arithmetic;
	bool is_relational;
	bool is_logical;
	bool is_bitwise;
};
struct UnOpTerm{
	struct ASTNode super; 
	//a term preceded by an unary operator.
	
	struct Op* op; //may be NULL
	
	struct Term* term;
};
struct Term{
	struct ASTNode super; 
	//only one of these is present,
	//check 'kind' for which it is.
	union myptr2 {

		struct Call*        m4;
		struct Expr*        m5;
		struct Variable*    m6;
		struct StringConst* m8;
		struct Lambda*      m11;
		struct ConstValue*  m12;
        struct MDirect* m13;
	} ptr;
	
	uint8_t kind; // 4 .. 13
};
struct MDirect{
    struct ASTNode super;
    struct Expr* expr;
};

#endif
