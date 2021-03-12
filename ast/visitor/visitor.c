#include <stdio.h>
#include <stdlib.h>

#include "../ast.h"
#include "visitor.h"

static void visitStructMember(struct StructMember* s, VISITOR);

static void visitStmt        (struct Stmt* s,       VISITOR);

//stmts
static void visitIfStmt      (struct IfStmt* i,     VISITOR);
static void visitWhileStmt   (struct WhileStmt* w,  VISITOR);
static void visitLoopStmt    (struct LoopStmt* l,   VISITOR);
static void visitForStmt     (struct ForStmt* f,    VISITOR);
static void visitAssignStmt  (struct AssignStmt* a, VISITOR);
static void visitSwitchStmt  (struct SwitchStmt* s, VISITOR);
static void visitCaseStmt    (struct CaseStmt* c,   VISITOR);
static void visitMethodCall  (struct MethodCall* m, VISITOR);
static void visitRetStmt     (struct RetStmt* r,    VISITOR);
static void visitBreakStmt   (VISITOR);
static void visitContinueStmt(VISITOR);

//expr
static void visitExpr        (struct Expr* e,       VISITOR);
static void visitOp          (struct Op* o,         VISITOR);
static void visitUnOpTerm    (struct UnOpTerm* u,   VISITOR);
static void visitTerm        (struct Term* t,       VISITOR);

//const
static void visitBoolConst   (struct BoolConst* b,  VISITOR);
static void visitIntConst    (struct IntConst* i,   VISITOR);
static void visitCharConst   (struct CharConst* c,  VISITOR);
static void visitFloatConst  (struct FloatConst* f, VISITOR);
static void visitHexConst    (struct HexConst* h,   VISITOR);
static void visitBinConst    (struct BinConst* b,   VISITOR);
static void visitStringConst (struct StringConst* s,VISITOR);

//var
static void visitVariable    (struct Variable* v,   VISITOR);
static void visitSimpleVar   (struct SimpleVar* v,  VISITOR);

//types
static void visitType        (struct Type* t,       VISITOR);
static void visitArrayType   (struct ArrayType* a,  VISITOR);
static void visitSubrType    (struct SubrType* s,   VISITOR);
static void visitBasicTypeWrapped(struct BasicTypeWrapped* b, VISITOR);
static void visitSimpleType  (struct SimpleType* s, VISITOR);
static void visitPrimitiveType(struct PrimitiveType* p, VISITOR);
static void visitStructType(struct StructType* s,   VISITOR);
static void visitTypeParam   (struct TypeParam* t,  VISITOR);

void visitAST(struct AST* ast, VISITOR){

	for(int i = 0; i < ast->count_namespaces; i++){
	
		visitNamespace(ast->namespaces[i], visitor);
	}
}

void visitNamespace(struct Namespace* n, VISITOR){

	visitor(n, NODE_NAMESPACE);
	
	for(int i = 0; i < n->count_structs; i++)
		{ visitStructDecl(n->structs[i], visitor); }
	
	for(int i = 0; i < n->count_methods; i++)
		{ visitMethod(n->methods[i], visitor); }
}

void visitStructDecl(struct StructDecl* s, VISITOR){
	
	visitor(s, NODE_STRUCTDECL);
	
	for(int i = 0; i < s->count_members; i++)
		{ visitStructMember(s->members[i], visitor); }
}

void visitMethod(struct Method* m, VISITOR){
	
	visitor(m, NODE_METHOD);

	visitStmtBlock(m->block, visitor);
}

void visitStmtBlock(struct StmtBlock* b, VISITOR){
	
	visitor(b, NODE_STMTBLOCK);
	
	for(int i = 0; i < b->count; i++)
		{ visitStmt(b->stmts[i], visitor); }
}

//----------- static functions ---------------

static void visitStructMember(struct StructMember* s, VISITOR){
	
	visitor(s, NODE_STRUCTMEMBER);
}

static void visitStmt(struct Stmt* s, VISITOR){
	
	visitor(s, NODE_STMT);
	
	switch(s->kind){
		
		case 0: visitLoopStmt  (s->ptr.m0, visitor); break;
		case 1: visitMethodCall(s->ptr.m1, visitor); break;
		case 2: visitWhileStmt (s->ptr.m2, visitor); break;
		case 3: visitIfStmt    (s->ptr.m3, visitor); break;
		case 4: visitRetStmt   (s->ptr.m4, visitor); break; 
		case 5: visitAssignStmt(s->ptr.m5, visitor); break;
		case 6: break; 
		case 7: visitForStmt   (s->ptr.m7, visitor); break;
		case 8: visitSwitchStmt(s->ptr.m8, visitor); break;
		
		case 99:
			if(s->isBreak)   { visitBreakStmt(visitor); }
			if(s->isContinue){ visitContinueStmt(visitor); }
			break;
		
		default: 
			printf("[Visitor] Fatal (1)\n"); exit(1);
			break;
	}
}

static void visitIfStmt(struct IfStmt* i, VISITOR){
	
	visitor(i, NODE_IFSTMT);
	
	visitStmtBlock(i->block, visitor);
	
	if(i->elseBlock != NULL)
		{ visitStmtBlock(i->elseBlock, visitor); }
}

static void visitWhileStmt(struct WhileStmt* w, VISITOR){
	
	visitor(w, NODE_WHILESTMT);
	
	visitStmtBlock(w->block, visitor);
}

static void visitLoopStmt(struct LoopStmt* l, VISITOR){
	
	visitor(l, NODE_LOOPSTMT);
	
	visitStmtBlock(l->block, visitor);
}

static void visitForStmt(struct ForStmt* l, VISITOR){
	
	visitor(l, NODE_FORSTMT);
	
	visitStmtBlock(l->block, visitor);
}

static void visitAssignStmt(struct AssignStmt* a, VISITOR){
	
	visitor(a, NODE_ASSIGNSTMT);
	
	if(a->optType != NULL)
		{ visitType(a->optType, visitor); }
	
	visitVariable(a->var, visitor);
	
	visitExpr(a->expr, visitor);
}

static void visitSwitchStmt(struct SwitchStmt* s, VISITOR){
	
	visitor(s, NODE_SWITCHSTMT);
	
	for(int i=0; i < s->count_cases; i++)
		{ visitCaseStmt(s->cases[i], visitor); }
}

static void visitCaseStmt(struct CaseStmt* c, VISITOR){
	
	visitor(c, NODE_CASESTMT);
	
	if(c->block != NULL)
		{ visitStmtBlock(c->block, visitor); }
}


static void visitMethodCall(struct MethodCall* m, VISITOR){
	
	visitor(m, NODE_METHODCALL);
	
	for(int i=0; i < m->count_args; i++)
		{ visitExpr(m->args[i], visitor); }
}

static void visitRetStmt(struct RetStmt* r,    VISITOR){
	
	visitor(r, NODE_RETSTMT);
	
	visitExpr(r->returnValue, visitor);
}

static void visitBreakStmt(VISITOR){
	
	visitor(NULL, NODE_BREAKSTMT);
}

static void visitContinueStmt(VISITOR){
	
	visitor(NULL, NODE_CONTINUESTMT);
}

static void visitExpr(struct Expr* e, VISITOR){
	
	visitor(e, NODE_EXPR);
	
	visitUnOpTerm(e->term1, visitor);
	
	if(e->op != NULL){
	
		visitOp(e->op, visitor);
		visitUnOpTerm(e->term2, visitor);
	}
}

static void visitOp(struct Op* o, VISITOR){
	
	visitor(o, NODE_OP);
}

static void visitUnOpTerm(struct UnOpTerm* u, VISITOR){
	
	visitor(u, NODE_UNOPTERM);
	
	visitOp(u->op, visitor);
	visitTerm(u->term, visitor);
}

static void visitTerm(struct Term* t, VISITOR){
	
	visitor(t, NODE_TERM);
	
	switch(t->kind){
	
		case 1: visitBoolConst(t->ptr.m1, visitor);  break;
		case 2: visitIntConst(t->ptr.m2, visitor);   break;
		case 3: visitCharConst(t->ptr.m3, visitor);  break;
		case 4: visitMethodCall(t->ptr.m4, visitor); break;
		case 5: visitExpr(t->ptr.m5, visitor);       break;
		case 6: visitVariable(t->ptr.m6, visitor);   break;
		case 7: visitFloatConst(t->ptr.m7, visitor); break;
		case 8: visitStringConst(t->ptr.m8, visitor);break;
		case 9: visitHexConst(t->ptr.m9, visitor);   break;
		case 10: visitBinConst(t->ptr.m10, visitor); break;
		
		default:
			printf("[Visitor] Fatal(2)\n"); 
			exit(1);
			break;
	}
}

static void visitBoolConst(struct BoolConst* b,  VISITOR){
	
	visitor(b, NODE_BOOLCONST);
}

static void visitIntConst(struct IntConst* i,   VISITOR){
	
	visitor(i, NODE_INTCONST);
}

static void visitCharConst(struct CharConst* c,  VISITOR){
	
	visitor(c, NODE_CHARCONST);
}

static void visitFloatConst(struct FloatConst* f, VISITOR){
	
	visitor(f, NODE_FLOATCONST);
}

static void visitHexConst(struct HexConst* h,   VISITOR){
	
	visitor(h, NODE_HEXCONST);
}

static void visitBinConst(struct BinConst* b,   VISITOR){
	
	visitor(b, NODE_BINCONST);
}

static void visitStringConst(struct StringConst* s,VISITOR){
	
	visitor(s, NODE_STRINGCONST);
}


static void visitVariable(struct Variable* v,   VISITOR){
	
	visitor(v, NODE_VARIABLE);
	
	visitSimpleVar(v->simpleVar, visitor);
	
	if(v->memberAccess != NULL)
		{ visitVariable(v->memberAccess, visitor); }
}

static void visitSimpleVar(struct SimpleVar* v,  VISITOR){
	
	visitor(v, NODE_SIMPLEVAR);
	
	for(int i=0; i < v->count_indices; i++)
		{ visitExpr(v->indices[i], visitor); }
}


static void visitType(struct Type* t, VISITOR){
		
	visitor(t, NODE_TYPE);
	
	if(t->m1 != NULL)
		{ visitBasicTypeWrapped(t->m1, visitor); }
		
	if(t->m2 != NULL)
		{ visitTypeParam(t->m2, visitor); }
		
	if(t->m3 != NULL)
		{ visitArrayType(t->m3, visitor); }
}

static void visitArrayType(struct ArrayType* a, VISITOR){
		
	visitor(a, NODE_ARRAYTYPE);
	
	visitType(a->element_type, visitor);
}

static void visitSubrType(struct SubrType* s, VISITOR){
		
	visitor(s, NODE_SUBRTYPE);
	
	visitType(s->returnType, visitor);
	
	for(int i=0; i < s->count_argTypes; i++)
		{ visitType(s->argTypes[i], visitor); }
}

static void visitBasicTypeWrapped(struct BasicTypeWrapped* b, VISITOR){
	
	visitor(b, NODE_BASICWRAPPEDTYPE);
	
	if(b->simpleType != NULL)
		{ visitSimpleType(b->simpleType, visitor); }
		
	if(b->subrType != NULL)
		{ visitSubrType(b->subrType, visitor); }
}

static void visitSimpleType(struct SimpleType* s, VISITOR){
		
	visitor(s, NODE_SIMPLETYPE);
	
	if(s->primitiveType != NULL)
		{ visitPrimitiveType(s->primitiveType, visitor); }
		
	if(s->structType != NULL)
		{ visitStructType(s->structType, visitor); }
}

static void visitPrimitiveType(struct PrimitiveType* p,VISITOR){
		
	visitor(p, NODE_PRIMITIVETYPE);
}

static void visitStructType(struct StructType* s, VISITOR){
		
	visitor(s, NODE_STRUCTTYPE);
}

static void visitTypeParam(struct TypeParam* t, VISITOR){
		
	visitor(t, NODE_TYPEPARAM);
}

