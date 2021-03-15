#ifndef COPYAST
#define COPYAST

#include "../ast.h"

//this header file declares all subroutines
//for copying the memory allocated for AST Nodes.

//the subroutines here perform 
//DEEP COPIES
//as this is sometimes required for AST Nodes

//const
struct BoolConst*   copyBoolConst(struct BoolConst* bc);
struct CharConst*   copyCharConst(struct CharConst* cc);
struct FloatConst*  copyFloatConst(struct FloatConst* fc);
struct IntConst*    copyIntConst(struct IntConst* ic);
struct HexConst*    copyHexConst(struct HexConst* hc);
struct BinConst*    copyBinConst(struct BinConst* hc);
struct StringConst* copyStringConst(struct StringConst* s);

//stmt
struct MethodCall*  copyMethodCall(struct MethodCall* c);
struct IfStmt*      copyIfStmt(struct IfStmt* i);
struct AssignStmt*  copyAssignStmt(struct AssignStmt* a);
struct RetStmt*     copyRetStmt(struct RetStmt* r);
struct Stmt*        copyStmt(struct Stmt* stmt);
struct WhileStmt*   copyWhileStmt(struct WhileStmt* w);
struct LoopStmt*    copyLoopStmt(struct LoopStmt* l);
struct ForStmt*     copyForStmt(struct ForStmt* f);
struct SwitchStmt*  copySwitchStmt(struct SwitchStmt* s);
struct CaseStmt*    copyCaseStmt(struct CaseStmt* c);

//types
struct Type*             copyType(struct Type* t);
struct TypeParam*        copyTypeParam(struct TypeParam* t);
struct SubrType*         copySubrType(struct SubrType* s);
struct SimpleType*       copySimpleType(struct SimpleType* s);
struct StructType*       copyStructType(struct StructType* s);
struct PrimitiveType*    copyPrimitiveType(struct PrimitiveType* p);
struct BasicTypeWrapped* copyBasicTypeWrapped(struct BasicTypeWrapped* b);
struct ArrayType*        copyArrayType(struct ArrayType* a);

//expr
struct Expr*     copyExpr(struct Expr* expr);
struct Term*     copyTerm(struct Term* t);
struct UnOpTerm* copyUnOpTerm(struct UnOpTerm* t);
struct Op*       copyOp(struct Op* op);

//var
struct SimpleVar* copySimpleVar(struct SimpleVar* sv);
struct Variable*  copyVariable(struct Variable* var);

//subr
struct Method*  copyMethod(struct Method* m);
struct DeclArg* copyDeclArg(struct DeclArg* d);

//struct
struct StructDecl*   copyStructDecl(struct StructDecl* s);
struct StructMember* copyStructMember(struct StructMember* s);

//other
struct Identifier* copyIdentifier(struct Identifier* id);
struct Range*      copyRange(struct Range* r);
struct StmtBlock*  copyStmtBlock(struct StmtBlock* s);
struct Namespace*  copyNamespace(struct Namespace* n);

struct AST* copyAST(struct AST* ast);
#endif
