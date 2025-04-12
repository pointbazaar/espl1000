#pragma once

#include "../ast_declare.h"

enum OP {
	OP_NONE, //empty, no operator

	OP_PLUS,
	OP_MINUS,
	OP_MULTIPLY,
	OP_DIV,
	OP_MOD, // %

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

enum TERM_KIND {

	TERM_KIND_CALL,
	TERM_KIND_EXPR,
	TERM_KIND_VAR,
	TERM_KIND_STRINGCONST,
	TERM_KIND_CONSTVALUE,
	TERM_KIND_ENUM_VALUE,
};

struct Term {
	struct ASTNode super;

	//only one of these is present,
	//check 'kind' for which it is.
	union myptr2 {

		struct Call* call_term;
		struct Expr* expr_term;
		struct Variable* var_term;
		struct StringConst* stringconst_term;
		struct ConstValue* constvalue_term;
		char* enum_value_term;
	} ptr;

	enum TERM_KIND kind;
};
