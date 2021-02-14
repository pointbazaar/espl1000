#ifndef COPYAST
#define COPYAST

#include "ast.h"

//this header file declares all subroutines
//for copying the memory allocated for AST Nodes.

//the subroutines here perform 
//DEEP COPIES
//as this is sometimes required for AST Nodes

//not every AST Node is implemented yet though

struct BoolConst* copyBoolConst(struct BoolConst* bc);
struct CharConst* copyCharConst(struct CharConst* cc);
struct Expr* copyExpr(struct Expr* expr);
struct FloatConst* copyFloatConst(struct FloatConst* fc);
struct Identifier* copyIdentifier(struct Identifier* id);
struct IntConst* copyIntConst(struct IntConst* ic);
struct HexConst* copyHexConst(struct HexConst* hc);
struct SimpleVar* copySimpleVar(struct SimpleVar* sv);
struct Term* copyTerm(struct Term* t);
struct UnOpTerm* copyUnOpTerm(struct UnOpTerm* t);
struct Variable* copyVariable(struct Variable* var);
struct Op* copyOp(struct Op* op);
struct StringConst* copyStringConst(struct StringConst* s);
struct MethodCall* copyMethodCall(struct MethodCall* c);

#endif
