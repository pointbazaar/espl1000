#ifndef AST_H
#define AST_H

#include <stdbool.h>

struct AST_Whole_Program;

struct Namespace;
struct Method;
struct StructDecl;

struct TokenList;
struct Op;
struct Term;
struct Identifier;
struct Expr;
struct Variable;
struct SimpleVar;

//const
struct BoolConst;
struct CharConst;
struct IntConst;
struct FloatConst;

//statements
struct AssignStmt;
struct IfStmt;
struct MethodCall;
struct RetStmt;
struct Stmt;
struct WhileStmt;

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
	int count_namespaces;
};
struct BoolConst{
	bool value;
};
struct CharConst {
	char content;
};
struct DeclArg  {
	struct Type* type;

	bool has_name;
	char name[20];
};
struct Expr {
	struct Term* term1;

	//these 2 may be NULL
	struct Op* op;
	struct Term* term2;
};
struct FloatConst{
	float value;
};
struct Identifier  {

	char* identifier;
};
struct IntConst {
	int value;
};
struct Method {

	bool isPublic;
	bool hasSideEffects;

	struct Type* returnType;
	char* methodName;

	int count_arguments;
	struct DeclArg** arguments;

	int count_statements;
	struct Stmt** statements;
};
struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char* srcPath;
	char* name;

	//structs must be declared before the subroutines
	struct Method** methods;
	int count_methods;

	struct StructDecl** structs;
	int count_structs;
};
struct Op {
	char* op;
};
struct SimpleVar {
	char* name;
	
	//may be NULL
	struct Expr* indexOptional;
};
struct StructDecl{
	struct StructMember** members;
	int count_members;
};
struct StructMember{
	struct Type* type;
	char* name;
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
};
struct Variable {
	struct SimpleVar* simpleVariableNode;

	struct Variable** memberAccessList;
	int count_memberAccessList;
};
struct AssignStmt {
	struct Type* optTypeNode;	//may be NULL (optional)

	struct Variable* variableNode;
	struct Expr* expressionNode;
};
struct IfStmt{
	struct Expr* condition;

	int count_statements;
	struct Stmt** statements;

	int count_elseStatements;
	struct Stmt** elseStatements;
};
struct MethodCall {
	char* methodName;

	//arguments to the subroutine call
	int count_args;
	struct Expr** args;
};
struct RetStmt{
	struct Expr* returnValue;
};
struct Stmt {
	//only one of those will be != NULL
	struct MethodCall* m1;
	struct WhileStmt* m2;
	struct IfStmt* m3;
	struct RetStmt* m4;
	struct AssignStmt* m5;
};
struct WhileStmt  {
	struct Expr* condition;
	int count_statements;
	struct Stmt** statements;
};
struct ArrayType {
	struct Type* element_type;
};
struct BasicTypeWrapped {
	//these are alternatives,
	//to replace my use of <variant> which was
	//causing me problems
	//only one of these is != NULL
	struct SimpleType* m1;
	struct SubrType* m2;
};
struct SimpleType {
	//the typeName is not a 'char*',
	//because this way we have less indirection
	//and hopefully more performance

	//can have at most 31 characters,
	//but this limit is arbitrary
	char typeName[32];
};
struct SubrType {
	struct Type* returnType;
	bool hasSideEffects;

	struct Type** argumentTypes;
	int count_argumentTypes;
};
struct Type {
	//only one of these is != NULL
	struct BasicTypeWrapped* m1;
	struct TypeParam* m2;
	struct ArrayType* m3;
};
struct TypeParam {
	int typeParameterIndex;
};
#endif
