#include <stdio.h>
#include <stdlib.h>

#include "../ast.h"
#include "visitor.h"

static void visitDeclArg        (struct DeclArg* da,   VISITOR, void* arg);

//struct
static void visitStructMember	(struct StructMember* s, VISITOR, void* arg);

//other
static void visitStmt        	(struct Stmt* s,       VISITOR, void* arg);
static void visitLambda			(struct Lambda* l,     VISITOR, void* arg);

//stmts
static void visitIfStmt      	(struct IfStmt* i,     VISITOR, void* arg);
static void visitWhileStmt   	(struct WhileStmt* w,  VISITOR, void* arg);
static void visitLoopStmt    	(struct LoopStmt* l,   VISITOR, void* arg);
static void visitForStmt     	(struct ForStmt* f,    VISITOR, void* arg);
static void visitAssignStmt  	(struct AssignStmt* a, VISITOR, void* arg);
static void visitSwitchStmt  	(struct SwitchStmt* s, VISITOR, void* arg);
static void visitCaseStmt    	(struct CaseStmt* c,   VISITOR, void* arg);
static void visitCall           (struct Call* m,       VISITOR, void* arg);
static void visitRetStmt     	(struct RetStmt* r,    VISITOR, void* arg);
static void visitBreakStmt   	(VISITOR, void* arg);
static void visitContinueStmt	(VISITOR, void* arg);
static void visitThrowStmt      (VISITOR, void* arg);
static void visitTryCatchStmt   (struct TryCatchStmt* t, VISITOR, void* arg);

//expr
static void visitExpr        	(struct Expr* e,       VISITOR, void* arg);
static void visitOp          	(struct Op* o,         VISITOR, void* arg);
static void visitUnOpTerm    	(struct UnOpTerm* u,   VISITOR, void* arg);
static void visitTerm       	(struct Term* t,       VISITOR, void* arg);

//const
static void visitBoolConst   	(struct BoolConst* b,  VISITOR, void* arg);
static void visitIntConst    	(struct IntConst* i,   VISITOR, void* arg);
static void visitCharConst   	(struct CharConst* c,  VISITOR, void* arg);
static void visitFloatConst  	(struct FloatConst* f, VISITOR, void* arg);
static void visitHexConst    	(struct HexConst* h,   VISITOR, void* arg);
static void visitBinConst    	(struct BinConst* b,   VISITOR, void* arg);
static void visitStringConst 	(struct StringConst* s,VISITOR, void* arg);

//var
static void visitVariable    	(struct Variable* v,   VISITOR, void* arg);
static void visitSimpleVar   	(struct SimpleVar* v,  VISITOR, void* arg);

//types
static void visitType        	(struct Type* t,          VISITOR, void* arg);
static void visitArrayType   	(struct ArrayType* a,     VISITOR, void* arg);
static void visitSubrType    	(struct SubrType* s,      VISITOR, void* arg);
static void visitBasicType      (struct BasicType* b,     VISITOR, void* arg);
static void visitSimpleType  	(struct SimpleType* s,    VISITOR, void* arg);
static void visitPrimitiveType	(struct PrimitiveType* p, VISITOR, void* arg);
static void visitStructType		(struct StructType* s,    VISITOR, void* arg);
static void visitTypeParam   	(struct TypeParam* t,     VISITOR, void* arg);

void visitAST(struct AST* ast, VISITOR, void* arg){

	for(int i = 0; i < ast->count_namespaces; i++)
		{ visitNamespace(ast->namespaces[i], visitor, arg); }
}

void visitNamespace(struct Namespace* n, VISITOR, void* arg){

	visitor(n, NODE_NAMESPACE, arg);
	
	for(int i = 0; i < n->count_structs; i++)
		{ visitStructDecl(n->structs[i], visitor, arg); }
	
	for(int i = 0; i < n->count_methods; i++)
		{ visitMethod(n->methods[i], visitor, arg); }
}

void visitStructDecl(struct StructDecl* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRUCTDECL, arg);
	
	for(int i = 0; i < s->count_members; i++)
		{ visitStructMember(s->members[i], visitor, arg); }
}

void visitMethod(struct Method* m, VISITOR, void* arg){
	
	visitor(m, NODE_METHOD, arg);
	
	for(uint32_t i = 0; i < m->count_args; i++){
		visitDeclArg(m->args[i], visitor, arg);
	}
	
	visitType(m->return_type, visitor, arg);

	visitStmtBlock(m->block, visitor, arg);
}

void visitStmtBlock(struct StmtBlock* b, VISITOR, void* arg){
	
	visitor(b, NODE_STMTBLOCK, arg);
	
	for(int i = 0; i < b->count; i++)
		{ visitStmt(b->stmts[i], visitor, arg); }
}

//----------- static functions ---------------

static void visitDeclArg(struct DeclArg* da, VISITOR, void* arg){
	
	visitor(da, NODE_DECLARG, arg);
	
	visitType(da->type, visitor, arg);
}

static void visitStructMember(struct StructMember* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRUCTMEMBER, arg);
}

static void visitStmt(struct Stmt* s, VISITOR, void* arg){
	
	visitor(s, NODE_STMT, arg);
	
	switch(s->kind){
		
		case 0: visitLoopStmt  (s->ptr.m0, visitor, arg);   break;
		case 1: visitCall      (s->ptr.m1, visitor, arg);   break;
		case 2: visitWhileStmt (s->ptr.m2, visitor, arg);   break;
		case 3: visitIfStmt    (s->ptr.m3, visitor, arg);   break;
		case 4: visitRetStmt   (s->ptr.m4, visitor, arg);   break; 
		case 5: visitAssignStmt(s->ptr.m5, visitor, arg);   break;
		case 6: visitTryCatchStmt(s->ptr.m6, visitor, arg); break;
		case 7: visitForStmt   (s->ptr.m7, visitor, arg);   break;
		case 8: visitSwitchStmt(s->ptr.m8, visitor, arg);   break;
		
		case 99:
			if(s->is_break)   { visitBreakStmt(visitor, arg); }
			if(s->is_continue){ visitContinueStmt(visitor, arg); }
			if(s->is_throw)   { visitThrowStmt(visitor, arg); }
			break;
		
		default: 
			printf("[Visitor] Fatal (1)\n"); exit(1);
			break;
	}
}

static void visitLambda (struct Lambda* l, VISITOR, void* arg){

	visitor(l, NODE_LAMBDA, arg);
	
	visitExpr(l->expr, visitor, arg);
}

static void visitIfStmt(struct IfStmt* i, VISITOR, void* arg){
	
	visitor(i, NODE_IFSTMT, arg);
	
	visitExpr(i->condition, visitor, arg);
	visitStmtBlock(i->block, visitor, arg);
	
	if(i->else_block != NULL)
		{ visitStmtBlock(i->else_block, visitor, arg); }
}

static void visitWhileStmt(struct WhileStmt* w, VISITOR, void* arg){
	
	visitor(w, NODE_WHILESTMT, arg);
	
	visitStmtBlock(w->block, visitor, arg);
}

static void visitLoopStmt(struct LoopStmt* l, VISITOR, void* arg){
	
	visitor(l, NODE_LOOPSTMT, arg);
	
	visitStmtBlock(l->block, visitor, arg);
}

static void visitForStmt(struct ForStmt* l, VISITOR, void* arg){
	
	visitor(l, NODE_FORSTMT, arg);
	
	visitStmtBlock(l->block, visitor, arg);
}

static void visitAssignStmt(struct AssignStmt* a, VISITOR, void* arg){
	
	visitor(a, NODE_ASSIGNSTMT, arg);
	
	if(a->opt_type != NULL)
		{ visitType(a->opt_type, visitor, arg); }
	
	visitVariable(a->var, visitor, arg);
	
	visitExpr(a->expr, visitor, arg);
}

static void visitSwitchStmt(struct SwitchStmt* s, VISITOR, void* arg){
	
	visitor(s, NODE_SWITCHSTMT, arg);
	
	visitExpr(s->expr, visitor, arg);
	
	for(int i = 0; i < s->count_cases; i++)
		{ visitCaseStmt(s->cases[i], visitor, arg); }
}

static void visitCaseStmt(struct CaseStmt* c, VISITOR, void* arg){
	
	visitor(c, NODE_CASESTMT, arg);
	
	if(c->block != NULL)
		{ visitStmtBlock(c->block, visitor, arg); }
}

static void visitCall(struct Call* m, VISITOR, void* arg){
	
	visitor(m, NODE_CALL, arg);
	
	for(int i = 0; i < m->count_args; i++)
		{ visitExpr(m->args[i], visitor, arg); }
}

static void visitRetStmt(struct RetStmt* r, VISITOR, void* arg){
	
	visitor(r, NODE_RETSTMT, arg);
	
	visitExpr(r->return_value, visitor, arg);
}

static void visitBreakStmt(VISITOR, void* arg){
	
	visitor(NULL, NODE_BREAKSTMT, arg);
}

static void visitContinueStmt(VISITOR, void* arg){
	
	visitor(NULL, NODE_CONTINUESTMT, arg);
}

static void visitThrowStmt(VISITOR, void* arg){
	
	visitor(NULL, NODE_THROWSTMT, arg);
}

static void visitTryCatchStmt(struct TryCatchStmt* t, VISITOR, void* arg){
	
	visitor(t, NODE_TRYCATCHSTMT, arg);
	
	visitStmtBlock(t->try_block, visitor, arg);
	visitStmtBlock(t->catch_block, visitor, arg);
}

static void visitExpr(struct Expr* e, VISITOR, void* arg){
	
	visitor(e, NODE_EXPR, arg);
	
	visitUnOpTerm(e->term1, visitor, arg);
	
	if(e->op == NULL)
		{ return; }
	
	visitOp(e->op, visitor, arg);
	visitUnOpTerm(e->term2, visitor, arg);
}

static void visitOp(struct Op* o, VISITOR, void* arg){
	
	visitor(o, NODE_OP, arg);
}

static void visitUnOpTerm(struct UnOpTerm* u, VISITOR, void* arg){
	
	visitor(u, NODE_UNOPTERM, arg);
	
	visitOp(u->op, visitor, arg);
	visitTerm(u->term, visitor, arg);
}

static void visitTerm(struct Term* t, VISITOR, void* arg){
	
	visitor(t, NODE_TERM, arg);
	
	switch(t->kind){
	
		case 1: visitBoolConst(t->ptr.m1, visitor, arg);  break;
		case 2: visitIntConst(t->ptr.m2, visitor, arg);   break;
		case 3: visitCharConst(t->ptr.m3, visitor, arg);  break;
		case 4: visitCall(t->ptr.m4, visitor, arg);       break;
		case 5: visitExpr(t->ptr.m5, visitor, arg);       break;
		case 6: visitVariable(t->ptr.m6, visitor, arg);   break;
		case 7: visitFloatConst(t->ptr.m7, visitor, arg); break;
		case 8: visitStringConst(t->ptr.m8, visitor, arg);break;
		case 9: visitHexConst(t->ptr.m9, visitor, arg);   break;
		case 10: visitBinConst(t->ptr.m10, visitor, arg); break;
		case 11: visitLambda(t->ptr.m11, visitor, arg);   break;
		
		default:
			printf("[Visitor] Fatal(2)\n"); 
			exit(1);
			break;
	}
}

static void visitBoolConst(struct BoolConst* b, VISITOR, void* arg){
	
	visitor(b, NODE_BOOLCONST, arg);
}

static void visitIntConst(struct IntConst* i, VISITOR, void* arg){
	
	visitor(i, NODE_INTCONST, arg);
}

static void visitCharConst(struct CharConst* c, VISITOR, void* arg){
	
	visitor(c, NODE_CHARCONST, arg);
}

static void visitFloatConst(struct FloatConst* f, VISITOR, void* arg){
	
	visitor(f, NODE_FLOATCONST, arg);
}

static void visitHexConst(struct HexConst* h, VISITOR, void* arg){
	
	visitor(h, NODE_HEXCONST, arg);
}

static void visitBinConst(struct BinConst* b, VISITOR, void* arg){
	
	visitor(b, NODE_BINCONST, arg);
}

static void visitStringConst(struct StringConst* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRINGCONST, arg);
}

static void visitVariable(struct Variable* v, VISITOR, void* arg){
	
	visitor(v, NODE_VARIABLE, arg);
	
	visitSimpleVar(v->simple_var, visitor, arg);
	
	if(v->member_access != NULL)
		{ visitVariable(v->member_access, visitor, arg); }
}

static void visitSimpleVar(struct SimpleVar* v, VISITOR, void* arg){
	
	visitor(v, NODE_SIMPLEVAR, arg);
	
	for(int i = 0; i < v->count_indices; i++)
		{ visitExpr(v->indices[i], visitor, arg); }
}

static void visitType(struct Type* t, VISITOR, void* arg){
		
	visitor(t, NODE_TYPE, arg);
	
	if(t->m1 != NULL)
		{ visitBasicType(t->m1, visitor, arg); }
		
	if(t->m2 != NULL)
		{ visitTypeParam(t->m2, visitor, arg); }
		
	if(t->m3 != NULL)
		{ visitArrayType(t->m3, visitor, arg); }
}

static void visitArrayType(struct ArrayType* a, VISITOR, void* arg){
		
	visitor(a, NODE_ARRAYTYPE, arg);
	
	visitType(a->element_type, visitor, arg);
}

static void visitSubrType(struct SubrType* s, VISITOR, void* arg){
		
	visitor(s, NODE_SUBRTYPE, arg);
	
	visitType(s->return_type, visitor, arg);
	
	for(int i = 0; i < s->count_arg_types; i++)
		{ visitType(s->arg_types[i], visitor, arg); }
}

static void visitBasicType(struct BasicType* b, VISITOR, void* arg){
	
	visitor(b, NODE_BASICTYPE, arg);
	
	if(b->simple_type != NULL)
		{ visitSimpleType(b->simple_type, visitor, arg); }
		
	if(b->subr_type != NULL)
		{ visitSubrType(b->subr_type, visitor, arg); }
}

static void visitSimpleType(struct SimpleType* s, VISITOR, void* arg){
		
	visitor(s, NODE_SIMPLETYPE, arg);
	
	if(s->primitive_type != NULL)
		{ visitPrimitiveType(s->primitive_type, visitor, arg); }
		
	if(s->struct_type != NULL)
		{ visitStructType(s->struct_type, visitor, arg); }
}

static void visitPrimitiveType(struct PrimitiveType* p, VISITOR, void* arg){
		
	visitor(p, NODE_PRIMITIVETYPE, arg);
}

static void visitStructType(struct StructType* s, VISITOR, void* arg){
		
	visitor(s, NODE_STRUCTTYPE, arg);
}

static void visitTypeParam(struct TypeParam* t, VISITOR, void* arg){
		
	visitor(t, NODE_TYPEPARAM, arg);
}

