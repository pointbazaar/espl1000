#ifndef ASTWRITER
#define ASTWRITER
//../parsing
#include "../parsing/AST_Whole_Program.hpp"
#include "../parsing/Namespace.hpp"
#include "../parsing/Method.hpp"
#include "../parsing/DeclArg.hpp"
#include "../parsing/BoolConst.hpp"
#include "../parsing/CharConst.hpp"
#include "../parsing/Expr.hpp"
#include "../parsing/Identifier.hpp"
#include "../parsing/IntConst.hpp"
#include "../parsing/Op.hpp"
#include "../parsing/SimpleVar.hpp"
#include "../parsing/Term.hpp"
#include "../parsing/Variable.hpp"
//statements
#include "../parsing/statements/IfStmt.hpp"
#include "../parsing/statements/WhileStmt.hpp"
#include "../parsing/statements/RetStmt.hpp"
#include "../parsing/statements/AssignStmt.hpp"
#include "../parsing/statements/Stmt.hpp"
#include "../parsing/statements/MethodCall.hpp"
//typenodes
#include "../parsing/typenodes/ArrayType.hpp"
#include "../parsing/typenodes/BasicTypeWrapped.hpp"
#include "../parsing/typenodes/SimpleType.hpp"
#include "../parsing/typenodes/SubrType.hpp"
#include "../parsing/typenodes/Type.hpp"
#include "../parsing/typenodes/TypeParam.hpp"

void write_ast(string filename, struct Namespace* namespaceNode);

void write(struct Namespace* m, 	ofstream* file);
void write(struct Method* m, 		ofstream* file);
void write(struct DeclArg* m, 		ofstream* file);
void write(struct Expr* m, 			ofstream* file);
void write(struct Op* m, 			ofstream* file);
void write(struct IntConst* m, 		ofstream* file);
void write(struct BoolConst* m, 	ofstream* file);
void write(struct CharConst* m, 	ofstream* file);
void write(struct Variable* m, 		ofstream* file);
void write(struct SimpleVar* m, 	ofstream* file);
void write(struct Term* m, 			ofstream* file);

//statementnodes
void write(struct Stmt* m, 			ofstream* file);
void write(struct IfStmt* m, 		ofstream* file);
void write(struct WhileStmt* m, 	ofstream* file);
void write(struct RetStmt* m, 		ofstream* file);
void write(struct AssignStmt* m, 	ofstream* file);
void write(struct MethodCall* m, 	ofstream* file);

//typenodes
void write(struct Type* m, 				ofstream* file);
void write(struct SubrType* m, 			ofstream* file);
void write(struct SimpleType* m, 		ofstream* file);
void write(struct ArrayType* m, 		ofstream* file);
void write(struct TypeParam* m, 		ofstream* file);
void write(struct BasicTypeWrapped* m, 	ofstream* file);

#endif