
#include "ast/ast.h"
#include "tables/symtable.h"
#include "visitor.h"

#define VISITOR void(*visitor)(void*, enum NODE_TYPE)

static void visitNamespace   (struct Namespace* n,  VISITOR);

static void visitMethod      (struct Method* m,     VISITOR);
static void visitStructDecl  (struct StructDecl* s, VISITOR);
static void visitStructMember(struct StructMember* s, VISITOR);


static void visitStmtBlock   (struct StmtBlock* s,  VISITOR);
static void visitStmt        (struct Stmt* s,       VISITOR);

static void visitIfStmt      (struct IfStmt* i,     VISITOR);
static void visitWhileStmt   (struct WhileStmt* w,  VISITOR);
static void visitLoopStmt    (struct LoopStmt* l,   VISITOR);
static void visitForStmt     (struct ForStmt* f,    VISITOR);
static void visitAssignStmt  (struct AssignStmt* a, VISITOR);
static void visitSwitchStmt  (struct SwitchStmt* s, VISITOR);
static void visitCaseStmt    (struct CaseStmt* c,   VISITOR);

void visit_ast(struct AST* ast, VISITOR){

	for(int i = 0; i < ast->count_namespaces; i++){
	
		visitNamespace(ast->namespaces[i], visitor);
	}
}

//----------- static functions ---------------

static void visitNamespace(struct Namespace* n, VISITOR){

	visitor(n, NODE_NAMESPACE);
	
	for(int i = 0; i < n->count_structs; i++)
		{ visitStructDecl(n->structs[i], visitor); }
	
	for(int i = 0; i < n->count_methods; i++)
		{ visitMethod(n->methods[i], visitor); }
}

static void visitStructDecl(struct StructDecl* s, VISITOR){
	
	visitor(s, NODE_STRUCTDECL);
	
	for(int i = 0; i < s->count_members; i++)
		{ visitStructMember(s->members[i], visitor); }
}

static void visitStructMember(struct StructMember* s, VISITOR){
	
	visitor(s, NODE_STRUCTMEMBER);
}

static void visitMethod(struct Method* m, VISITOR){
	
	visitor(m, NODE_METHOD);

	visitStmtBlock(m->block, visitor);
}

static void visitStmtBlock(struct StmtBlock* b, VISITOR){
	
	visitor(b, NODE_STMTBLOCK);
	
	for(int i = 0; i < b->count; i++)
		{ visitStmt(b->stmts[i], visitor); }
}

static void visitStmt(struct Stmt* s, VISITOR){
	
	visitor(s, NODE_STMT);
	
	switch(s->kind){
		
		case 0: visitLoopStmt  (s->ptr.m0, visitor); break;
		case 1: break; 
		case 2: visitWhileStmt (s->ptr.m2, visitor); break;
		case 3: visitIfStmt    (s->ptr.m3, visitor); break;
		case 4: break; 
		case 5: visitAssignStmt(s->ptr.m5, visitor); break;
		case 6: break; 
		case 7: visitForStmt   (s->ptr.m7, visitor); break;
		case 8: visitSwitchStmt(s->ptr.m8, visitor); break;
		
		default: break;
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
