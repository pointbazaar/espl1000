#ifndef STR_AST_H
#define STR_AST_H

#include "../ast.h"

//this header file declares all subroutines
//for providing string representations
//of AST nodes. Meaning to convert them
//to their source code form.

//all char* returned are heap-allocated

//const
char* strBoolConst(struct BoolConst* bc);	
char* strCharConst(struct CharConst* cc);
char* strFloatConst(struct FloatConst* fc);
char* strIntConst(struct IntConst* ic);
char* strHexConst(struct HexConst* hc);
char* strStringConst(struct StringConst* s);
char* strBinConst(struct BinConst* b);

//types
char* strArrayType(struct ArrayType* at);
char* strBasicType(struct BasicType* btw);
char* strSimpleType(struct SimpleType* st);
char* strSubrType(struct SubrType* st);
char* strType(struct Type* t);
char* strTypeParam(struct TypeParam* t);
char* strPrimitiveType(struct PrimitiveType* p);
char* strStructType(struct StructType* s);

//struct
char* strStructDecl(struct StructDecl* s);
char* strStructMember(struct StructMember* s);

//var
char* strVariable(struct Variable* v);
char* strSimpleVar(struct SimpleVar* s);

//expr
char* strExpr(struct Expr* e);
char* strOp(struct Op* o);
char* strUnOpTerm(struct UnOpTerm* u);
char* strTerm(struct Term* t);

//stmts
char* strStmt(struct Stmt* stmt);

char* strAssignStmt(struct AssignStmt* a);
char* strForStmt(struct ForStmt* f);
char* strIfStmt(struct IfStmt* i);
char* strLoopStmt(struct LoopStmt* l);
char* strWhileStmt(struct WhileStmt* w);
char* strSwitchStmt(struct SwitchStmt* s);
char* strCall(struct Call* m);
char* strRetStmt(struct RetStmt* r);
char* strCaseStmt(struct CaseStmt* c);
char* strTryCatchStmt(struct TryCatchStmt* tcs);

//other
char* strIdentifier(struct Identifier* id);
char* strRange(struct Range* r);
char* strStmtBlock(struct StmtBlock* block);
char* strLambda(struct Lambda* l);

#endif
