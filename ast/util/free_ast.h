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

//const
void freeBoolConst(struct BoolConst* bc);	
void freeCharConst(struct CharConst* cc);
void freeFloatConst(struct FloatConst* fc);
void freeIntConst(struct IntConst* ic);
void freeHexConst(struct HexConst* hc);
void freeBinConst(struct BinConst* hc);
void freeStringConst(struct StringConst* s);

//subr
void freeDeclArg(struct DeclArg* da);
void freeMethod(struct Method* m);

//other
void freeIdentifier(struct Identifier* id);
void freeNamespace(struct Namespace* ns);
void freeStmtBlock(struct StmtBlock* block);
void freeRange(struct Range* range);
void freeLambda(struct Lambda* l);

//struct
void freeStructDecl(struct StructDecl* sd);
void freeStructMember(struct StructMember* sm);

//expr
void freeExpr(struct Expr* expr);
void freeTerm(struct Term* t);
void freeUnOpTerm(struct UnOpTerm* t);
void freeOp(struct Op* op);

//var
void freeVariable(struct Variable* var);
void freeSimpleVar(struct SimpleVar* sv);

//stmts
void freeAssignStmt(struct AssignStmt* as);
void freeIfStmt(struct IfStmt* is);
void freeLoopStmt(struct LoopStmt* is);
void freeCall(struct Call* mc);
void freeRetStmt(struct RetStmt* rs);
void freeStmt(struct Stmt* s);
void freeWhileStmt(struct WhileStmt* ws);
void freeForStmt(struct ForStmt* fstmt);
void freeSwitchStmt(struct SwitchStmt* sstmt);
void freeCaseStmt(struct CaseStmt* cstmt);

//types
void freeArrayType(struct ArrayType* at);
void freeBasicTypeWrapped(struct BasicTypeWrapped* btw);
void freeSimpleType(struct SimpleType* st);
void freeSubrType(struct SubrType* st);
void freeType(struct Type* t);
void freeTypeParam(struct TypeParam* tp);
void freePrimitiveType(struct PrimitiveType* p);
void freeStructType(struct StructType* s);


#endif
