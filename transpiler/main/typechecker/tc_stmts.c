#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

bool tc_stmt(struct Stmt* s, struct TCCtx* tcctx){

	tcctx->current_line_num = s->super.line_num;

	switch(s->kind){

        case 1: return tc_methodcall(s->ptr.m1,   tcctx);
        case 2: return tc_whilestmt(s->ptr.m2,    tcctx);
        case 3: return tc_ifstmt(s->ptr.m3,       tcctx);
        case 4: return tc_retstmt(s->ptr.m4,      tcctx);
        case 5: return tc_assignstmt(s->ptr.m5,   tcctx);
        case 6: return tc_trycatchstmt(s->ptr.m6, tcctx);
        case 7: return tc_forstmt(s->ptr.m7,      tcctx);
        case 8: return tc_switchstmt(s->ptr.m8,   tcctx);
		
		case 99:
			if(s->is_continue){ }
			if(s->is_break)   { }
            if(s->is_throw)   { return tc_throwstmt(s, tcctx); }
	}
    return true;
}

bool tc_throwstmt(struct Stmt* s, struct TCCtx* tcctx){
	
	struct Method* m = tcctx->current_fn;
				
	if(m->decl->throws) { return true; }
	
	//are we inside try-catch stmt?
	if(tcctx->depth_inside_try_stmt > 0){ return true; }
	
	char* s1 = str_stmt(s);
	char msg[100];
	sprintf(msg, "\t%s\n", s1);
	strcat(msg, ERR_NO_THROW_OUTSIDE_TRY_OR_THROWS_SUBR);
	
	free(s1);
	
	error(tcctx, msg, TC_ERR_THROW_WRONG_USAGE);
    return false;
}

bool tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx){
	
	tcctx->current_line_num = i->super.line_num;
	
	struct Type* type = infer_type_expr(tcctx->st, i->condition);
	
	if(!is_bool_type(type)){
		
		char* s1 = str_expr(i->condition);
		
		char msg[100];
		sprintf(msg, "\t%s\n", s1);
		strcat(msg, ERR_CONDITION_REQUIRES_BOOL);
		
		free(s1);
		
		error(tcctx, msg, TC_ERR_CONDITION_REQUIRES_BOOL);
        return false;
	}
	
    return tc_stmtblock(i->block, tcctx);
}

bool tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx){

	tcctx->current_line_num = w->super.line_num;

	struct Type* type = infer_type_expr(tcctx->st, w->condition);
	
	if(!is_bool_type(type)){
		
		char* s1 = str_expr(w->condition);
		
		char msg[200];
		sprintf(msg, "\t%s\n", s1);
		strcat(msg, ERR_CONDITION_REQUIRES_BOOL);
		
		free(s1);
		
		error(tcctx, msg, TC_ERR_CONDITION_REQUIRES_BOOL);
        return false;
	}
	
	tcctx->depth_inside_loop++;
	bool has_err = tc_stmtblock(w->block, tcctx);
	tcctx->depth_inside_loop--;

    return has_err;
}

bool tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx){

	tcctx->current_line_num = r->super.line_num;

	struct Type* returnType = tcctx->current_fn->decl->return_type;

    if(!tc_expr(r->return_value, tcctx)){return false;}

    struct Type* returnedType = infer_type_expr(tcctx->st, r->return_value);

	if(is_integer_type(returnType) && is_integer_type(returnedType))
		{ return true; }
		
	//do not check if returned expr
	//is a call to malloc
	if(is_malloc(r->return_value)){ return true; }
	
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
		
		error(tcctx, msg, TC_ERR_WRONG_RETURN_TYPE);
        return false;
	}

    return true;
}

bool tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx){

	tcctx->current_line_num = s->super.line_num;

	struct Type* type = infer_type_expr(tcctx->st, s->expr);
	
	if(!is_primitive_type(type)){
		
		char* s1 = str_expr(s->expr);
		
		char msg[200];
		sprintf(msg, "\tswitch %s\n", s1);
		strcat(msg, ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
		
		free(s1);
	
		error(tcctx, msg, TC_ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
        return false;
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
			
			error(tcctx, msg, TC_ERR_SWITCH_CASE_TYPE_MISMATCH);
            return false;
		}
	}

    return true;
}

bool tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx){
	
	tcctx->current_line_num = f->super.line_num;
	
    if(!tc_range(f->range, tcctx)){return false;}
	tcctx->depth_inside_loop++;
	bool is_ok = tc_stmtblock(f->block, tcctx);
	tcctx->depth_inside_loop--;
    return is_ok;
}

bool tc_trycatchstmt(struct TryCatchStmt* tcs, struct TCCtx* tcctx){
	
	tcctx->current_line_num = tcs->super.line_num;
	
	tcctx->depth_inside_try_stmt++;
	bool is_ok = tc_stmtblock(tcs->try_block, tcctx);
	tcctx->depth_inside_try_stmt--;

    if(!is_ok){return false;}
	
    return tc_stmtblock(tcs->catch_block, tcctx);
}
