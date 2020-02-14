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

void write_ast(string filename, Namespace namespaceNode);

void write(Namespace m, ofstream* file);
void write(Method m, ofstream* file);
void write(DeclArg m, ofstream* file);
void write(struct Expr* m, ofstream* file);
void write(Op m, ofstream* file);
void write(struct IntConst* m, ofstream* file);
void write(struct BoolConst* m, ofstream* file);
void write(struct CharConst* m, ofstream* file);
void write(struct Variable* m, ofstream* file);
void write(SimpleVar m, ofstream* file);
void write(Term m, ofstream* file);

//statementnodes
void write(Stmt m, ofstream* file);
void write(struct IfStmt* m, ofstream* file);
void write(WhileStmt m, ofstream* file);
void write(RetStmt m, ofstream* file);
void write(AssignStmt m, ofstream* file);
void write(struct MethodCall* m, ofstream* file);

//typenodes
void write(Type m, ofstream* file);
void write(SubrType m, ofstream* file);
void write(SimpleType m, ofstream* file);
void write(ArrayType m, ofstream* file);
void write(TypeParam m, ofstream* file);
void write(BasicTypeWrapped m, ofstream* file);

#endif