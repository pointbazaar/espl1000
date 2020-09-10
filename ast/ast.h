#ifndef AST_H
#define AST_H

#define DEFAULT_STR_SIZE 64 //64

#include <stdbool.h>
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

//const
struct BoolConst;
struct CharConst;
struct IntConst;
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
	int value;
};
struct StringConst {
	char value[100];
};
struct Method {
	bool isPublic;
	bool hasSideEffects;

	struct Type* returnType;
	char name[DEFAULT_STR_SIZE];

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
	
	//may be NULL
	struct Expr* optIndex;
};
struct StructDecl{
	//the name of the struct
	char name[DEFAULT_STR_SIZE];
	
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
	//only one of these may be != NULL
	struct BoolConst* m1;
	struct IntConst* m2;
	struct CharConst* m3;
	struct MethodCall* m4;
	struct Expr* m5;
	struct Variable* m6;
	struct FloatConst* m7;
	struct StringConst* m8;
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
	//only one of those will be != NULL
	struct LoopStmt* m0;
	struct MethodCall* m1;
	struct WhileStmt* m2;
	struct IfStmt* m3;
	struct RetStmt* m4;
	struct AssignStmt* m5;
	struct BreakStmt* m6;
	struct ForStmt* m7;
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
	//empty
};
struct SwitchStmt{
	struct Variable* var;
	
	unsigned int count_cases;
	struct CaseStmt* cases;
};
struct CaseStmt{
	
	//cases must have constant values
	//known at compile time
	//(to build the jump table)
	
	//one of these will be != NULL
	struct BoolConst* m1;
	struct CharConst* m2;
	struct IntConst* m3;
	
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
};
struct SubrType {
	struct Type* returnType;
	bool hasSideEffects;

	struct Type** argTypes;
	uint8_t count_argTypes;
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
