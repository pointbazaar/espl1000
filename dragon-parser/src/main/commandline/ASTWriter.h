#ifndef ASTWRITER
#define ASTWRITER
//../parsing
#include "../parsing/AST_Whole_Program.h"
#include "../parsing/Namespace.h"
#include "../parsing/Method.h"
#include "../parsing/DeclArg.h"
#include "../parsing/BoolConst.h"
#include "../parsing/CharConst.h"
#include "../parsing/FloatConst.h"
#include "../parsing/Expr.h"
#include "../parsing/Identifier.h"
#include "../parsing/IntConst.h"
#include "../parsing/Op.h"
#include "../parsing/SimpleVar.h"
#include "../parsing/Term.h"
#include "../parsing/Variable.h"
//statements
#include "../parsing/statements/IfStmt.h"
#include "../parsing/statements/WhileStmt.h"
#include "../parsing/statements/RetStmt.h"
#include "../parsing/statements/AssignStmt.h"
#include "../parsing/statements/Stmt.h"
#include "../parsing/statements/MethodCall.h"
//typenodes
#include "../parsing/typenodes/ArrayType.h"
#include "../parsing/typenodes/BasicTypeWrapped.h"
#include "../parsing/typenodes/SimpleType.h"
#include "../parsing/typenodes/SubrType.h"
#include "../parsing/typenodes/Type.h"
#include "../parsing/typenodes/TypeParam.h"


#include <stdlib.h>
#include <stdio.h>

void write_ast(char* filename, struct Namespace* namespaceNode);

void writeNamespace(struct Namespace* m, 	FILE* file);
void writeMethod(struct Method* m, 		FILE* file);
void writeDeclArg(struct DeclArg* m, 		FILE* file);
void writeExpr(struct Expr* m, 			FILE* file);
void writeOp(struct Op* m, 			FILE* file);
void writeIntConst(struct IntConst* m, 		FILE* file);
void writeBoolConst(struct BoolConst* m, 	FILE* file);
void writeCharConst(struct CharConst* m, 	FILE* file);
void writeFloatConst(struct FloatConst* m, 	FILE* file);
void writeVariable(struct Variable* m, 		FILE* file);
void writeSimpleVar(struct SimpleVar* m, 	FILE* file);
void writeTerm(struct Term* m, 			FILE* file);

//statementnodes
void writeStmt(struct Stmt* m, 			FILE* file);
void writeIfStmt(struct IfStmt* m, 		FILE* file);
void writeWhileStmt(struct WhileStmt* m, 	FILE* file);
void writeRetStmt(struct RetStmt* m, 		FILE* file);
void writeAssignStmt(struct AssignStmt* m, 	FILE* file);
void writeMethodCall(struct MethodCall* m, 	FILE* file);

//typenodes
void writeType(struct Type* m, 				FILE* file);
void writeSubrType(struct SubrType* m, 			FILE* file);
void writeSimpleType(struct SimpleType* m, 		FILE* file);
void writeArrayType(struct ArrayType* m, 		FILE* file);
void writeTypeParam(struct TypeParam* m, 		FILE* file);
void writeBasicTypeWrapped(struct BasicTypeWrapped* m, 	FILE* file);

#endif
