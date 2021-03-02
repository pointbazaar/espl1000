#ifndef FREEAST
#define FREEAST

#include "../ast.h"

//this header file declares all subroutines
//for freeing the memory allocated for AST Nodes.

//these subroutines are separated from the
//constructor subroutines in the parser,
//as other modules (transpiler, etc.)
//must also be able to free AST Nodes,
//as they could obtain such via ast_reader.c

void freeAST(struct AST* ast);

void freeBoolConst(struct BoolConst* bc);	

void freeCharConst(struct CharConst* cc);

void freeDeclArg(struct DeclArg* da);

void freeExpr(struct Expr* expr);

void freeFloatConst(struct FloatConst* fc);

void freeIdentifier(struct Identifier* id);

void freeIntConst(struct IntConst* ic);

void freeHexConst(struct HexConst* hc);

void freeBinConst(struct BinConst* hc);

void freeMethod(struct Method* m);

void freeNamespace(struct Namespace* ns);

void freeSimpleVar(struct SimpleVar* sv);

void freeStmtBlock(struct StmtBlock* block);

void freeStructDecl(struct StructDecl* sd);

void freeStructMember(struct StructMember* sm);

void freeTerm(struct Term* t);

void freeUnOpTerm(struct UnOpTerm* t);

void freeVariable(struct Variable* var);

void freeAssignStmt(struct AssignStmt* as);

void freeBreakStmt(struct BreakStmt* b);

void freeIfStmt(struct IfStmt* is);

void freeLoopStmt(struct LoopStmt* is);

void freeMethodCall(struct MethodCall* mc);

void freeRetStmt(struct RetStmt* rs);

void freeStmt(struct Stmt* s);

void freeWhileStmt(struct WhileStmt* ws);

void freeArrayType(struct ArrayType* at);

void freeBasicTypeWrapped(struct BasicTypeWrapped* btw);

void freeSimpleType(struct SimpleType* st);

void freeSubrType(struct SubrType* st);

void freeType(struct Type* t);

void freeTypeParam(struct TypeParam* tp);

void freeOp(struct Op* op);

void freeStringConst(struct StringConst* s);

void freeRange(struct Range* range);

void freeForStmt(struct ForStmt* fstmt);

void freeSwitchStmt(struct SwitchStmt* sstmt);

void freeCaseStmt(struct CaseStmt* cstmt);

#endif
