#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "free_ast.h"

//this file also contains static asserts
//about the sizes of AST structures.
//this is because any program which uses these
//AST structs has to also free them.

void freeAST_Whole_Program(struct AST_Whole_Program* ast) {
	
	static_assert(
		sizeof(struct AST_Whole_Program)
		<= sizeof(void*) + 8,
		   ""
	);

	for(int i=0; i < ast->count_namespaces; i++) {
		freeNamespace(ast->namespaces[i]);
	}
	free(ast->namespaces);
	free(ast);
}

void freeBoolConst(struct BoolConst* bc) {
	
	static_assert(
		sizeof(struct BoolConst) <= 8,
		   ""
	);
	
	free(bc);
}

void freeCharConst(struct CharConst* cc) {
	
	static_assert(
		sizeof(struct CharConst) <= 8,
		   ""
    );
	free(cc);
}

void freeDeclArg(struct DeclArg* da) {
	
	static_assert(
		sizeof(struct DeclArg) 
		<= sizeof(void*) + 1 + DEFAULT_STR_SIZE + 7,
		""
	);
	freeType(da->type);
	free(da);
}


void freeExpr(struct Expr* expr) {
	
	static_assert(sizeof(struct Expr) <= sizeof(void*)*3,"");
	
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
void freeHexConst(struct HexConst* hc){
	free(hc);
}

void freeMethod(struct Method* m) {
	
	static_assert(
		sizeof(struct Method) 
		<= sizeof(void*) * 3
		   + DEFAULT_STR_SIZE 
		   + 8,
		   ""
	);

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
	
	static_assert(
		sizeof(struct SimpleVar) 
		<= sizeof(void*) + 8 + DEFAULT_STR_SIZE,
		   ""
	);

	for(int i=0;i < sv->count_indices; i++){
		freeExpr(sv->indices[i]);
	}
	free(sv->indices);
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

	freeSimpleType(sd->type);
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
	
	static_assert(
		sizeof(struct Term) <= 8 + sizeof(void*),
		   ""
	);

	switch(t->kind){
		case 1: freeBoolConst  (t->ptr.m1); break;
		case 2: freeIntConst   (t->ptr.m2); break;
		case 3: freeCharConst  (t->ptr.m3); break;
		case 4: freeMethodCall (t->ptr.m4); break;
		case 5: freeExpr       (t->ptr.m5); break;
		case 6: freeVariable   (t->ptr.m6); break;
		case 7: freeFloatConst (t->ptr.m7); break;
		case 8: freeStringConst(t->ptr.m8); break;
		case 9: freeHexConst   (t->ptr.m9); break;
		default:
			printf("Error in freeTerm(...)\n");
			free(t);
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
	
	static_assert(
		sizeof(struct AssignStmt) 
		<= 8 + 3 * sizeof(void*),
		   ""
	);

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
	
	static_assert(
		sizeof(struct Stmt) <= 8 + sizeof(void*),
		   ""
	);

	switch(s->kind){
		
		case 0: freeLoopStmt(s->ptr.m0); break;
		case 1: freeMethodCall(s->ptr.m1); break;
		case 2: freeWhileStmt(s->ptr.m2); break;
		case 3: freeIfStmt(s->ptr.m3); break;
		case 4: freeRetStmt(s->ptr.m4); break;
		case 5: freeAssignStmt(s->ptr.m5); break;
		case 6: freeBreakStmt(s->ptr.m6); break;
		case 7: freeForStmt(s->ptr.m7); break;
		case 8: freeSwitchStmt(s->ptr.m8); break;
		default:
			printf("Error in freeStmt\n");
			free(s);
			exit(1);
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
	if(st->typeParamCount != 0){free(st->typeParams);}
	free(st);
}

void freeSubrType(struct SubrType* st) {
	
	static_assert(
		sizeof(struct SubrType) <= 2 * sizeof(void*) + 8,
		   ""
	);
	
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
	
	static_assert(
		sizeof(struct Op) 
		<= sizeof(void*),
		   ""
	);
	
	free(op);
}

void freeStringConst(struct StringConst* s){
	free(s->value);
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
	
	static_assert(
		sizeof(struct SwitchStmt) <= 3 * sizeof(void*),
		   ""
	);
	
	freeVariable(s->var);
	for(int i=0; i < s->count_cases; i++){
		freeCaseStmt(s->cases[i]);
	}
	free(s->cases);
	free(s);
}

void freeCaseStmt(struct CaseStmt* c){
	if(c->block != NULL){
		freeStmtBlock(c->block);
	}
	switch(c->kind){
		case 0: freeBoolConst(c->ptr.m1); break;
		case 1: freeCharConst(c->ptr.m2); break;
		case 2: freeIntConst(c->ptr.m3); break;
		default:
			printf("Error in freeCaseStmt\n");
			exit(1);
	}
	free(c);
}
