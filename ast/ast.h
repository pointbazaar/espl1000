#ifndef AST_H
#define AST_H

/*
I determined in code experiments 
that up to around 128 bytes there is a speed penalty
for using the indirection of char* instead
of having a fixed size char array in your structure.
Having many strings inline wastes memory
but is better for execution time.
*/
//for variables, method names, ...
#define DEFAULT_STR_SIZE 32 

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

struct AST_Whole_Program;

struct Namespace;
struct Method;
struct StructDecl;

struct StmtBlock;

struct TokenList;
struct Op;
struct Term;
struct Expr;
struct UnOpTerm;
struct Identifier;
struct Variable;
struct SimpleVar;
struct Range;
struct DeclArg;

//const
struct BoolConst;
struct CharConst;
struct IntConst;
struct HexConst;
struct BinConst;
struct FloatConst;
struct StringConst;

//statements
struct AssignStmt;
struct IfStmt;
struct MethodCall;
struct RetStmt;
struct Stmt;
struct WhileStmt;
struct LoopStmt;
struct BreakStmt;
struct ForStmt;
struct SwitchStmt;
struct CaseStmt;

//typenodes
struct Type;
struct ArrayType;
struct BasicTypeWrapped;
struct SimpleType;
struct SubrType;
struct TypeParam;

struct AST_Whole_Program  {
	//this contains all namespace nodes for the whole program

	struct Namespace** namespaces;
	uint16_t count_namespaces;
};
struct BoolConst{
	bool value;
};
struct CharConst {
	char value;
};
struct DeclArg  {
	struct Type* type;

	bool has_name;
	char name[DEFAULT_STR_SIZE];
};
struct Expr {
	struct UnOpTerm* term1;

	//these 2 may be NULL
	struct Op* op;
	struct UnOpTerm* term2;
	
};
struct Range {
	struct Expr* start;
	struct Expr* end;
};
//--------------
struct FloatConst{
	float value;
};
struct Identifier  {
	char identifier[DEFAULT_STR_SIZE];
};
struct IntConst {
	int32_t value;
};
struct HexConst {
	uint32_t value;
};
struct BinConst {
	uint32_t value;
};
struct StringConst {
	//string constants
	//should not have limited size
	//therefore it is a pointer
	char* value;
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
struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char srcPath[DEFAULT_STR_SIZE];
	char name[DEFAULT_STR_SIZE];

	//structs must be declared before the subroutines
	struct Method** methods;
	uint16_t count_methods;
	size_t capacity_methods;

	struct StructDecl** structs;
	uint16_t count_structs;
	size_t capacity_structs;
};
struct StmtBlock {
	uint16_t count;
	struct Stmt** stmts;
};
struct Op {
	//6 chars, as operators are mostly small
	char op[6];
};
struct SimpleVar {
	char name[DEFAULT_STR_SIZE];
	
	//indices
	uint8_t count_indices;
	struct Expr** indices;
};
struct StructDecl{
	//the type(and name) of the struct
	struct SimpleType* type;
	
	struct StructMember** members;
	uint16_t count_members;
};
struct StructMember{
	struct Type* type;
	char name[DEFAULT_STR_SIZE];
};
struct UnOpTerm{
	//'UnaryOpTerm'
	//a term preceded by an unary operator
	
	//can only be an unary operator,
	//may be NULL, if the term does not have a 
	//unary operator preceding it
	struct Op* op; 
	
	struct Term* term;
};
struct Term{
	
	//only one of these is present,
	//check 'kind' for which it is.
	union myptr2 {
		struct BoolConst* m1;
		struct IntConst* m2;
		struct CharConst* m3;
		struct MethodCall* m4;
		struct Expr* m5;
		struct Variable* m6;
		struct FloatConst* m7;
		struct StringConst* m8;
		struct HexConst* m9;
		struct BinConst* m10;
	} ptr;
	
	// = mX where X is from 1 .. 10
	uint8_t kind;
};
struct Variable {
	struct SimpleVar* simpleVar;

	struct Variable** memberAccessList;
	uint8_t count_memberAccessList;
};
//--------------------------------------
struct AssignStmt {
	struct Type* optType;	//may be NULL (optional)

	struct Variable* var;
	
	//the assignment operator [=,+=,-=, ...]
	char assign_op[3];
	
	struct Expr* expr;
};
struct IfStmt{
	struct Expr* condition;

	struct StmtBlock* block;
	
	//may be NULL
	struct StmtBlock* elseBlock;
};
struct MethodCall {
	char methodName[DEFAULT_STR_SIZE];

	//arguments to the subroutine call
	uint8_t count_args;
	struct Expr** args;
};
struct RetStmt{
	struct Expr* returnValue;
};
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
		struct BreakStmt* m6;
		struct ForStmt* m7;
		struct SwitchStmt* m8;
	} ptr;
	
	//0-based
	uint8_t kind;
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
struct BreakStmt {
	//empty, but we need one member
	//otherwise gcc gives warning on '-pedantic'
	int __should_not_be_used;
};
struct SwitchStmt{
	struct Variable* var;
	
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
	
	//may be NULL
	struct StmtBlock* block;
};
//-------------------------
struct ArrayType {
	struct Type* element_type;
};
struct BasicTypeWrapped {
	//these are alternatives,
	//to replace my use of <variant> which was
	//causing me problems
	//only one of these is != NULL
	struct SimpleType* simpleType;
	struct SubrType* subrType;
};
struct SimpleType {
	//the typeName is not a 'char*',
	//because this way we have less indirection
	//and hopefully more performance

	//can have at most 31 characters,
	//but this limit is arbitrary
	char typeName[DEFAULT_STR_SIZE];
	
	//the number of the type parameters
	uint8_t typeParamCount;
	uint8_t* typeParams;
};
struct SubrType {
	struct Type* returnType;
	
	struct Type** argTypes;
	uint8_t count_argTypes;
	
	bool hasSideEffects;
};
struct Type {
	//only one of these is != NULL
	struct BasicTypeWrapped* m1;
	struct TypeParam* m2;
	struct ArrayType* m3;
};
struct TypeParam {
	//the type parameter index
	uint8_t index;
};
#endif
