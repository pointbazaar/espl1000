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

void tc_stmt(struct Stmt* s){

	switch(s->kind){
	
		case 0: tc_loopstmt(s->ptr.m0);   break;
		case 1: tc_methodcall(s->ptr.m1); break;
		case 2: tc_whilestmt(s->ptr.m2);  break;
		case 3: tc_ifstmt(s->ptr.m3);     break;
		case 4: tc_retstmt(s->ptr.m4);    break;
		case 5: tc_assignstmt(s->ptr.m5); break;
		
		case 7: tc_forstmt(s->ptr.m7);    break;
		case 8: tc_switchstmt(s->ptr.m8); break;
	}
}

void tc_assignstmt(struct AssignStmt* a){

	//we make an exception
	//TODO: only make exception for array types
	//and other applicable types
	if(is_malloc(a->expr))
		{ return; }

	struct Type* rightType = 
		infer_type_expr(myst, a->expr);
	
	struct Type* leftType = a->optType;
	
	if(a->optType == NULL){
		leftType  = infer_type_variable(myst, a->var);
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
	
		error(ERR_ASSIGN_TYPES_MISMATCH);
	}
}

void tc_methodcall(struct MethodCall* m){

	struct SSTLine* line = 
		sst_get(myst->sst, m->methodName);
	
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
		error(ERR_NUM_ARGS);
	}
	
	for(uint8_t i = 0; i < expect_args; i++){
	
		struct Type* expect_type = method->args[i]->type;
		
		struct Type* actual_type = 
			infer_type_expr(myst, m->args[i]);
			
		if(is_integer_type(expect_type) 
		&& is_integer_type(actual_type))
			{ continue; }
		
		if(!eq_type(expect_type, actual_type)){
			error(ERR_ARG_TYPES);
		}
	}
}

void tc_ifstmt(struct IfStmt* i){
	
	struct Type* type = 
		infer_type_expr(myst, i->condition);
	
	if(!is_bool_type(type)){
		error(ERR_CONDITION_REQUIRES_BOOL);
	}
}

void tc_whilestmt(struct WhileStmt* w){

	struct Type* type = 
		infer_type_expr(myst, w->condition);
	
	if(!is_bool_type(type)){
		error(ERR_CONDITION_REQUIRES_BOOL);
	}
}

void tc_loopstmt(struct LoopStmt* l){

	struct Type* type = 
		infer_type_expr(myst, l->count);
	
	if(!is_integer_type(type)){
		error(ERR_LOOP_REQUIRES_INT);
	}
}

void tc_retstmt(struct RetStmt* r){

	struct Type* returnType = currentFn->returnType;
	
	struct Type* returnedType = 
		infer_type_expr(myst, r->returnValue);
	
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
		
		error(ERR_RETURN_TYPE_MISMATCH);
	}
}

void tc_switchstmt(struct SwitchStmt* s){

	struct Type* type = 
		infer_type_expr(myst, s->expr);
	
	if(!is_primitive_type(type)){
	
		error(ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
	}
	
	for(uint16_t i = 0; i < s->count_cases; i++){
	
		struct CaseStmt* c = s->cases[i];
		
		const bool isBool = c->kind == 0;
		const bool isChar = c->kind == 1;
		const bool isInt  = c->kind == 2;
		
		if(isBool && !is_bool_type(type)){
			
			error(ERR_CASE_TYPE_MISMATCH);
		}
	
		if(isChar && !is_char_type(type)){
			
			error(ERR_CASE_TYPE_MISMATCH);
		}
		
		if(isInt && !is_integer_type(type)){
			
			error(ERR_CASE_TYPE_MISMATCH);
		}
	}
}

void tc_forstmt(struct ForStmt* f){
	
	tc_range(f->range);
	tc_stmtblock(f->block);
}
