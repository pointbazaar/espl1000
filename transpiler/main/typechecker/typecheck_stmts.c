#include <string.h>
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
#include "typecheck_methodcall.h"
#include "typecheck_assignstmt.h"
#include "typecheck_expr.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_stmt(struct Stmt* s, struct TCCtx* tcctx){

	tcctx->current_line_num = s->super.line_num;

	switch(s->kind){
	
		case 0: tc_loopstmt(s->ptr.m0,   tcctx); break;
		case 1: tc_methodcall(s->ptr.m1, tcctx); break;
		case 2: tc_whilestmt(s->ptr.m2,  tcctx); break;
		case 3: tc_ifstmt(s->ptr.m3,     tcctx); break;
		case 4: tc_retstmt(s->ptr.m4,    tcctx); break;
		case 5: tc_assignstmt(s->ptr.m5, tcctx); break;
		case 6: tc_trycatchstmt(s->ptr.m6, tcctx); break;
		case 7: tc_forstmt(s->ptr.m7,    tcctx); break;
		case 8: tc_switchstmt(s->ptr.m8, tcctx); break;
		
		case 99:
			if(s->is_continue){ tc_continuestmt(s, tcctx); }
			if(s->is_break)   { tc_breakstmt(s, tcctx); }
			if(s->is_throw)   { tc_throwstmt(s, tcctx); }
	}
}

void tc_throwstmt(struct Stmt* s, struct TCCtx* tcctx){
	
	struct Method* m = tcctx->current_fn;
				
	if(m->decl->throws) { return; }
	
	//are we inside try-catch stmt?
	if(tcctx->depth_inside_try_stmt > 0){ return; }
	
	char* s1 = str_stmt(s);
	char msg[100];
	sprintf(msg, "\t%s\n", s1);
	strcat(msg, ERR_NO_THROW_OUTSIDE_TRY_OR_THROWS_SUBR);
	
	free(s1);
	
	error(tcctx, msg);
}

void tc_breakstmt(struct Stmt* s, struct TCCtx* tcctx){
	
	//TODO: check that we are in some loop 
}

void tc_continuestmt(struct Stmt* s, struct TCCtx* tcctx){
	
	//TODO: check that we are in some loop 
}

void tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx){
	
	tcctx->current_line_num = i->super.line_num;
	
	struct Type* type = 
		infer_type_expr(tcctx->current_filename, tcctx->st, i->condition);
	
	if(!is_bool_type(type)){
		
		char* s1 = str_expr(i->condition);
		
		char msg[100];
		sprintf(msg, "\t%s\n", s1);
		strcat(msg, ERR_CONDITION_REQUIRES_BOOL);
		
		free(s1);
		
		error(tcctx, msg);
	}
	
	tc_stmtblock(i->block, tcctx);
}

void tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx){

	tcctx->current_line_num = w->super.line_num;

	struct Type* type = 
		infer_type_expr(tcctx->current_filename, tcctx->st, w->condition);
	
	if(!is_bool_type(type)){
		
		char* s1 = str_expr(w->condition);
		
		char msg[200];
		sprintf(msg, "\t%s\n", s1);
		strcat(msg, ERR_CONDITION_REQUIRES_BOOL);
		
		free(s1);
		
		error(tcctx, msg);
	}
	
	tcctx->depth_inside_loop++;
	tc_stmtblock(w->block, tcctx);
	tcctx->depth_inside_loop--;
}

void tc_loopstmt(struct LoopStmt* l, struct TCCtx* tcctx){

	tcctx->current_line_num = l->super.line_num;

	struct Type* type = 
		infer_type_expr(tcctx->current_filename, tcctx->st, l->count);
	
	if(!is_integer_type(type)){
		
		char* s1 = str_expr(l->count);
		
		char msg[200];
		sprintf(msg, "\tloop %s\n", s1);
		strcat(msg, ERR_LOOP_REQUIRES_INT);
		
		free(s1);
		
		error(tcctx, msg);
	}
	
	tcctx->depth_inside_loop++;
	tc_stmtblock(l->block, tcctx);
	tcctx->depth_inside_loop--;
}

void tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx){

	tcctx->current_line_num = r->super.line_num;

	struct Type* returnType = tcctx->current_fn->decl->return_type;
	
	struct Type* returnedType = 
		infer_type_expr(tcctx->current_filename, tcctx->st, r->return_value);
	
	tc_expr(r->return_value, tcctx);
	
	if(is_integer_type(returnType) 
	&& is_integer_type(returnedType))
		{ return; }
		
	//do not check if returned expr
	//is a call to malloc
	if(is_malloc(r->return_value)){ return; }
	
	if(!eq_type(returnType, returnedType)){
		
		char* s1 = str_type(returnType);
		char* s2 = str_type(returnedType);
		char* s3 = str_ret_stmt(r);
		
		char msg[200];
		sprintf(msg, "\t%s\nexpected type: %s, actual type: %s\n", s3, s1, s2);
		strcat(msg, ERR_RETURN_TYPE_MISMATCH);

		free(s1);
		free(s2);
		free(s3);
		
		error(tcctx, msg);
	}
}

void tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx){

	tcctx->current_line_num = s->super.line_num;

	struct Type* type = 
		infer_type_expr(tcctx->current_filename, tcctx->st, s->expr);
	
	if(!is_primitive_type(type)){
		
		char* s1 = str_expr(s->expr);
		
		char msg[200];
		sprintf(msg, "\tswitch %s\n", s1);
		strcat(msg, ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
		
		free(s1);
	
		error(tcctx, msg);
	}
	
	for(uint32_t i = 0; i < s->count_cases; i++){
	
		struct CaseStmt* c = s->cases[i];

		const bool isBool = c->const_value->kind == 1;
		const bool isInt  = c->const_value->kind == 2;
		const bool isChar = c->const_value->kind == 3;
		
		bool isErr = false;
		
		if(isBool && !is_bool_type(type)){ isErr = true; }
	
		if(isChar && !is_char_type(type)){ isErr = true; }
		
		if(isInt && !is_integer_type(type)){ isErr = true; }
		
		if(isErr){
			
			char* s1 = str_case_stmt(c);
			
			char msg[200];
			sprintf(msg, "\t%s\n", s1);
			strcat(msg, ERR_CASE_TYPE_MISMATCH);
			
			free(s1);
			
			error(tcctx, msg);
		}
	}
}

void tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx){
	
	tcctx->current_line_num = f->super.line_num;
	
	tc_range(f->range, tcctx);
	tcctx->depth_inside_loop++;
	tc_stmtblock(f->block, tcctx);
	tcctx->depth_inside_loop--;
}

void tc_trycatchstmt(struct TryCatchStmt* tcs, struct TCCtx* tcctx){
	
	tcctx->current_line_num = tcs->super.line_num;
	
	tcctx->depth_inside_try_stmt++;
	tc_stmtblock(tcs->try_block, tcctx);
	tcctx->depth_inside_try_stmt--;
	
	tc_stmtblock(tcs->catch_block, tcctx);
}
