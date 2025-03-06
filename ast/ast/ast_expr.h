#pragma once

#include "../ast_declare.h"

enum OP {
	OP_NONE, //empty, no operator

	OP_PLUS,
	OP_MINUS,
	OP_MULTIPLY,

	OP_SHIFT_LEFT,
	OP_SHIFT_RIGHT,

	OP_OR, //|
	OP_AND, //&
	OP_XOR, //^

	//unary
	OP_NOT, //!
	OP_COMPLEMENT, //~

	//relational
	OP_EQ,
	OP_NEQ,
	OP_GE,
	OP_GT,
	OP_LE,
	OP_LT,

	OP_NUMBER_OF_OPS, //number of operators defined
};

struct Expr {
	struct ASTNode super;
	struct UnOpTerm* term1;
	enum OP op; //may be OP_NONE
	struct UnOpTerm* term2; //may be NULL
};
struct Deref {
	struct ASTNode super;
	struct Term* term;
};
struct AddressOf {
	struct ASTNode super;
	struct Term* term;
};
struct LValue {
	struct ASTNode super;

	// may be NULL
	struct Deref* deref;
	// may be NULL
	struct Variable* var;
};
struct UnOpTerm {
	struct ASTNode super;
	//a term preceded by an unary operator.

	// may be NULL
	struct AddressOf* address_of;

	// may be NULL
	struct Deref* deref;

	enum OP op; //may be OP_NONE

	// may be NULL
	struct Term* term;
};
struct Term {
	struct ASTNode super;
	//only one of these is present,
	//check 'kind' for which it is.
	union myptr2 {

		struct Call* m4;
		struct Expr* m5;
		struct Variable* m6;
		struct StringConst* m8;
		struct ConstValue* m12;
	} ptr;

	uint8_t kind; // 4 .. 12
};
