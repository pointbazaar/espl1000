#pragma once

#include "token/TokenKeys.h"

#include <inttypes.h>

//--- TOPLEVEL ---
struct ASTNode;

struct AST;
struct Namespace;
struct Method;
struct StructDecl;
struct EnumDecl;
struct MethodDecl;
//---------------
struct StmtBlock;
enum OP;
struct Term;
struct Deref;
struct AddressOf;
struct LValue;
struct Expr;
struct UnOpTerm;
struct Id;
struct Variable;
struct SimpleVar;
struct Range;
struct DeclArg;
//const
struct ConstValue;
struct StringConst;
//statements
struct AssignStmt;
struct IfStmt;
struct Call;
struct RetStmt;
struct Stmt;
struct WhileStmt;
struct BreakStmt;
struct ForStmt;
//typenodes
struct Type;
struct PointerType;
struct ArrayType;
struct BasicType;
struct SimpleType;
struct SubrType;
struct TypeParam;
struct PrimitiveType;
struct StructType;

//-----------
struct ASTNode {
	//Base Type from which all ASTNodes inherit
	//except {AST, Namespace}

	/*
	 Example:
	 struct Method {
	        struct ASTNode super;
	        ...
	 }
	 */

	uint32_t line_num;

	uint32_t annotations;
	//examples for (maybe) future annotations:
	// @liveroot   // for dead-analyzer
	// @halts      // for subroutines
	// @noalloc    // for subroutines
	// @nobranch   // for subroutines
	// @atleast(x) // for variables
	// @atmost(x)  // for variables
	// @condition(...)
	// @packed     // for structs
};

#define has_annotation(annotations, annotation) ((annotations) & (1 << ((annotation) - _ANNOT_START_))) != 0
