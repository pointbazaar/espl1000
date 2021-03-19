#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck_errors.h"
#include "typecheck_utils.h"
#include "typecheck_stmts.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_stmt(struct Stmt* s, struct TCCtx* tcctx){

	switch(s->kind){
	
		case 0: tc_loopstmt(s->ptr.m0,   tcctx); break;
		case 1: tc_methodcall(s->ptr.m1, tcctx); break;
		case 2: tc_whilestmt(s->ptr.m2,  tcctx); break;
		case 3: tc_ifstmt(s->ptr.m3,     tcctx); break;
		case 4: tc_retstmt(s->ptr.m4,    tcctx); break;
		case 5: tc_assignstmt(s->ptr.m5, tcctx); break;
		
		case 7: tc_forstmt(s->ptr.m7,    tcctx); break;
		case 8: tc_switchstmt(s->ptr.m8, tcctx); break;
	}
}

void tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx){

	//we make an exception
	//TODO: only make exception for array types
	//and other applicable types
	if(is_malloc(a->expr))
		{ return; }

	struct Type* rightType = 
		infer_type_expr(tcctx->st, a->expr);
	
	struct Type* leftType = a->optType;
	
	if(a->optType == NULL){
		leftType  = infer_type_variable(tcctx->st, a->var);
	}
	
	if(is_integer_type(leftType) 
	&& is_integer_type(rightType))
		{ return; }
	
	if(!eq_type(leftType, rightType)){
	
		char* str_t1 = strType(leftType);
		char* str_t2 = strType(rightType);
		
		char* str_a  = strAssignStmt(a);
		
		printf("    %s\n", str_a);
		
		printf("type1=%s, type2=%s\n", str_t1, str_t2);
		
		free(str_t1);
		free(str_t2);
		free(str_a);
	
		error(tcctx, ERR_ASSIGN_TYPES_MISMATCH);
	}
}

void tc_methodcall(struct Call* m, struct TCCtx* tcctx){

	struct SSTLine* line = sst_get(tcctx->st->sst, m->name);
	
	if(line->method == NULL){
		printf("[Typechecker][TODO]\n");
		printf("	line->method == NULL\n");
		return;
	}
	assert(line->method != NULL);
	
	struct Method* method = line->method;
	
	const uint8_t expect_args = method->count_args;
	const uint8_t actual_args = m->count_args;
	
	if(actual_args != expect_args){
		error(tcctx, ERR_NUM_ARGS);
	}
	
	for(uint8_t i = 0; i < expect_args; i++){
	
		struct Type* expect_type = method->args[i]->type;
		
		struct Type* actual_type = 
			infer_type_expr(tcctx->st, m->args[i]);
			
		if(is_integer_type(expect_type) 
		&& is_integer_type(actual_type))
			{ continue; }
		
		if(!eq_type(expect_type, actual_type)){
			error(tcctx, ERR_ARG_TYPES);
		}
	}
}

void tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx){
	
	struct Type* type = 
		infer_type_expr(tcctx->st, i->condition);
	
	if(!is_bool_type(type)){
		error(tcctx, ERR_CONDITION_REQUIRES_BOOL);
	}
}

void tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx){

	struct Type* type = 
		infer_type_expr(tcctx->st, w->condition);
	
	if(!is_bool_type(type)){
		error(tcctx, ERR_CONDITION_REQUIRES_BOOL);
	}
}

void tc_loopstmt(struct LoopStmt* l, struct TCCtx* tcctx){

	struct Type* type = 
		infer_type_expr(tcctx->st, l->count);
	
	if(!is_integer_type(type)){
		error(tcctx, ERR_LOOP_REQUIRES_INT);
	}
}

void tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx){

	struct Type* returnType = tcctx->currentFn->returnType;
	
	struct Type* returnedType = 
		infer_type_expr(tcctx->st, r->returnValue);
	
	if(is_integer_type(returnType) 
	&& is_integer_type(returnedType))
		{ return; }
	
	if(!eq_type(returnType, returnedType)){
		
		char* s1 = strType(returnType);
		char* s2 = strType(returnedType);
		
		char* s3 = strRetStmt(r);
		
		printf("type1=%s, type2=%s\n", s1, s2);
		
		printf("	%s\n", s3);
		
		free(s1);
		free(s2);
		free(s3);
		
		error(tcctx, ERR_RETURN_TYPE_MISMATCH);
	}
}

void tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx){

	struct Type* type = 
		infer_type_expr(tcctx->st, s->expr);
	
	if(!is_primitive_type(type)){
	
		error(tcctx, ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
	}
	
	for(uint16_t i = 0; i < s->count_cases; i++){
	
		struct CaseStmt* c = s->cases[i];
		
		const bool isBool = c->kind == 0;
		const bool isChar = c->kind == 1;
		const bool isInt  = c->kind == 2;
		
		if(isBool && !is_bool_type(type)){
			
			error(tcctx, ERR_CASE_TYPE_MISMATCH);
		}
	
		if(isChar && !is_char_type(type)){
			
			error(tcctx, ERR_CASE_TYPE_MISMATCH);
		}
		
		if(isInt && !is_integer_type(type)){
			
			error(tcctx, ERR_CASE_TYPE_MISMATCH);
		}
	}
}

void tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx){
	
	tc_range(f->range, tcctx);
	tc_stmtblock(f->block, tcctx);
}
