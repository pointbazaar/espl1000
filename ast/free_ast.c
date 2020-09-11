#include <stdlib.h>
#include <stdio.h>

#include "free_ast.h"

void freeAST_Whole_Program(struct AST_Whole_Program* ast) {

	for(int i=0; i < ast->count_namespaces; i++) {
		freeNamespace(ast->namespaces[i]);
	}
	free(ast->namespaces);
	free(ast);
}

void freeBoolConst(struct BoolConst* bc) {
	free(bc);
}

void freeCharConst(struct CharConst* cc) {
	free(cc);
}

void freeDeclArg(struct DeclArg* da) {
	freeType(da->type);
	free(da);
}


void freeExpr(struct Expr* expr) {
	freeUnOpTerm(expr->term1);
	if(expr->op != NULL) {
		freeOp(expr->op);
		freeUnOpTerm(expr->term2);
	}
	
	free(expr);
}


void freeFloatConst(struct FloatConst* fc) {
	free(fc);
}
void freeIdentifier(struct Identifier* id) {
	free(id);
}
void freeIntConst(struct IntConst* ic) {
	free(ic);
}

void freeMethod(struct Method* m) {

	freeType(m->returnType);
	for(int i=0; i < m->count_args; i++) {
		freeDeclArg(m->args[i]);
	}
	free(m->args);

	freeStmtBlock(m->block);
	free(m);
}

void freeNamespace(struct Namespace* ns) {

	for(int i=0; i < ns->count_methods; i++) {
		freeMethod(ns->methods[i]);
	}

	for(int i=0; i < ns->count_structs; i++) {
		freeStructDecl(ns->structs[i]);
	}

	free(ns->methods);
	free(ns->structs);

	free(ns);
}

void freeSimpleVar(struct SimpleVar* sv) {

	if(sv->optIndex != NULL) {
		freeExpr(sv->optIndex);
	}
	free(sv);
}

void freeStmtBlock(struct StmtBlock* block) {

	for(int i=0; i < block->count; i++) {
		freeStmt(block->stmts[i]);
	}
	free(block->stmts);
	free(block);
}

void freeStructDecl(struct StructDecl* sd) {

	for(int i=0; i < sd->count_members; i++) {
		freeStructMember(sd->members[i]);
	}
	free(sd);
}

void freeStructMember(struct StructMember* sm) {
	freeType(sm->type);
	free(sm);
}
void freeTerm(struct Term* t) {

	if(t->m1 != NULL) {
		freeBoolConst(t->m1);
	} else if(t->m2 != NULL) {
		freeIntConst(t->m2);
	} else if(t->m3 != NULL) {
		freeCharConst(t->m3);
	} else if(t->m4 != NULL) {
		freeMethodCall(t->m4);
	} else if(t->m5 != NULL) {
		freeExpr(t->m5);
	} else if(t->m6 != NULL) {
		freeVariable(t->m6);
	} else if(t->m7 != NULL) {
		freeFloatConst(t->m7);
	} else if(t->m8 != NULL) {
		freeStringConst(t->m8);
	} else {
		printf("Error in freeTerm(...)\n");
		exit(1);
	}
	free(t);
}
void freeUnOpTerm(struct UnOpTerm* t) {

	freeOp(t->op);
	freeTerm(t->term);
	free(t);
}

void freeVariable(struct Variable* var) {

	freeSimpleVar(var->simpleVar);
	for(int i=0; i < var->count_memberAccessList; i++) {
		freeVariable(var->memberAccessList[i]);
	}
	free(var->memberAccessList);
	free(var);
}

void freeAssignStmt(struct AssignStmt* as) {

	if(as->optType != NULL) {
		freeType(as->optType);
	}
	freeVariable(as->var);
	freeExpr(as->expr);

	free(as);
}
void freeBreakStmt(struct BreakStmt* b) {

	free(b);
}


void freeIfStmt(struct IfStmt* is) {

	freeExpr(is->condition);

	freeStmtBlock(is->block);
	if(is->elseBlock != NULL) {
		freeStmtBlock(is->elseBlock);
	}

	free(is);
}

void freeLoopStmt(struct LoopStmt* ls) {

	freeExpr(ls->count);
	freeStmtBlock(ls->block);
	free(ls);
}

void freeMethodCall(struct MethodCall* mc) {

	for(int i=0; i < mc->count_args; i++) {
		freeExpr(mc->args[i]);
	}
	free(mc->args);

	free(mc);
}

void freeRetStmt(struct RetStmt* rs) {

	if(rs->returnValue != NULL) {
		freeExpr(rs->returnValue);
	}
	free(rs);
}

void freeStmt(struct Stmt* s) {

	if(s->m0 != NULL) {
		freeLoopStmt(s->m0);

	} else if(s->m1 != NULL) {
		freeMethodCall(s->m1);

	} else if(s->m2 != NULL) {
		freeWhileStmt(s->m2);

	} else if(s->m3 != NULL) {
		freeIfStmt(s->m3);

	} else if(s->m4 != NULL) {
		freeRetStmt(s->m4);

	} else if(s->m5 != NULL) {
		freeAssignStmt(s->m5);
		
	} else if(s->m6 != NULL) {
		freeBreakStmt(s->m6);
		
	} else if(s->m7 != NULL) {
		freeForStmt(s->m7);
		
	} else if(s->m8 != NULL) {
		freeSwitchStmt(s->m8);
	}

	free(s);
}


void freeWhileStmt(struct WhileStmt* ws) {
	freeExpr(ws->condition);
	freeStmtBlock(ws->block);
	free(ws);
}

void freeArrayType(struct ArrayType* at) {
	freeType(at->element_type);
	free(at);
}

void freeBasicTypeWrapped(struct BasicTypeWrapped* btw) {

	if(btw->simpleType != NULL) {
		freeSimpleType(btw->simpleType);

	} else if(btw->subrType != NULL) {
		freeSubrType(btw->subrType);
	}

	free(btw);
}

void freeSimpleType(struct SimpleType* st) {

	free(st);
}

void freeSubrType(struct SubrType* st) {
	freeType(st->returnType);
	for(int i=0; i < st->count_argTypes; i++) {
		freeType(st->argTypes[i]);
	}
	free(st->argTypes);
	free(st);
}

void freeType(struct Type* t) {

	if(t->m1 != NULL) {
		freeBasicTypeWrapped(t->m1);
	} else if(t->m2 != NULL) {
		freeTypeParam(t->m2);
	} else if(t->m3 != NULL) {
		freeArrayType(t->m3);
	}
	free(t);
}

void freeTypeParam(struct TypeParam* tp) {

	free(tp);
}

void freeOp(struct Op* op){
	free(op);
}

void freeStringConst(struct StringConst* s){
	free(s);
}

void freeRange(struct Range* range){
	freeExpr(range->start);
	freeExpr(range->end);
	free(range);
}

void freeForStmt(struct ForStmt* fstmt){
	freeRange(fstmt->range);
	freeStmtBlock(fstmt->block);
	free(fstmt);
}

void freeSwitchStmt(struct SwitchStmt* s){
	freeVariable(s->var);
	for(int i=0; i < s->count_cases; i++){
		freeCaseStmt(s->cases[i]);
	}
}

void freeCaseStmt(struct CaseStmt* c){
	if(c->block != NULL){
		freeStmtBlock(c->block);
	}
	
	if(c->m1 != NULL){
		freeBoolConst(c->m1);
	}else if(c->m2 != NULL){
		freeCharConst(c->m2);
	}else if(c->m3 != NULL){
		freeIntConst(c->m3);
	}
}
