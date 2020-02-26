#ifndef ASTWRITER
#define ASTWRITER
//../parsing
#include "../parsing/AST_Whole_Program.h"
#include "../parsing/Namespace.h"
#include "../parsing/Method.h"
#include "../parsing/DeclArg.h"
#include "../parsing/BoolConst.h"
#include "../parsing/CharConst.h"
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

void write(struct Namespace* m, 	FILE* file);
void write(struct Method* m, 		FILE* file);
void write(struct DeclArg* m, 		FILE* file);
void write(struct Expr* m, 			FILE* file);
void write(struct Op* m, 			FILE* file);
void write(struct IntConst* m, 		FILE* file);
void write(struct BoolConst* m, 	FILE* file);
void write(struct CharConst* m, 	FILE* file);
void write(struct Variable* m, 		FILE* file);
void write(struct SimpleVar* m, 	FILE* file);
void write(struct Term* m, 			FILE* file);

//statementnodes
void write(struct Stmt* m, 			FILE* file);
void write(struct IfStmt* m, 		FILE* file);
void write(struct WhileStmt* m, 	FILE* file);
void write(struct RetStmt* m, 		FILE* file);
void write(struct AssignStmt* m, 	FILE* file);
void write(struct MethodCall* m, 	FILE* file);

//typenodes
void write(struct Type* m, 				FILE* file);
void write(struct SubrType* m, 			FILE* file);
void write(struct SimpleType* m, 		FILE* file);
void write(struct ArrayType* m, 		FILE* file);
void write(struct TypeParam* m, 		FILE* file);
void write(struct BasicTypeWrapped* m, 	FILE* file);

#endif