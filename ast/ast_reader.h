#ifndef ASTREADER
#define ASTREADER

#include "ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct AST_Whole_Program* readAST(char* filename, bool debug);

struct Namespace* readNamespace(FILE* file, bool debug);
struct Method* readMethod(FILE* file, bool debug);
struct StructDecl* readStructDecl(FILE* file, bool debug);
struct StructMember* readStructMember(FILE* file, bool debug);

// ------------------------
struct DeclArg* readDeclArg(FILE* file);
struct Expr* readExpr(FILE* file);
struct Op* readOp(FILE* file);
struct IntConst* readIntConst(FILE* file);
struct BoolConst* readBoolConst(FILE* file);
struct CharConst* readCharConst(FILE* file);
struct FloatConst* readFloatConst(FILE* file);
struct Variable* readVariable(FILE* file);
struct SimpleVar* readSimpleVar(FILE* file);
struct Term* readTerm(FILE* file);

//statementnodes
struct Stmt* readStmt(FILE* file);
struct IfStmt* readIfStmt(FILE* file);
struct WhileStmt* readWhileStmt(FILE* file);
struct RetStmt* readRetStmt(FILE* file);
struct AssignStmt* readAssignStmt(FILE* file);
struct MethodCall* readMethodCall(FILE* file);

//typenodes
struct Type* readType(FILE* file);
struct SubrType* readSubrType(FILE* file);
struct SimpleType* readSimpleType(FILE* file);
struct ArrayType* readArrayType(FILE* file);
struct TypeParam* readTypeParam(FILE* file);
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file);

#endif
