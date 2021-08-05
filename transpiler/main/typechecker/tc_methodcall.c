#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

#define ERR_CALLED_SIDE_EFFECT_IN_PURE "called '~>' subr in '->' subr."
#define ERR_SUBROUTINE_NOT_IN_SST_LVST "subroutine neither in SST nor LVST"

static bool tc_methodcall_args(
	struct Call* m, 
	struct Type** expect_types, 
	uint8_t expect_args, 
	struct TCCtx* tcctx
);

static bool tc_methodcall_arg(
	struct Call* m, 
	struct Type* expect_type,
	struct Expr* actual_expr,
	struct TCCtx* tcctx
);

static bool check_throw_rules(bool callee_throws, struct TCCtx* tcctx);

bool tc_methodcall(struct Call* m, struct TCCtx* tcctx){

	tcctx->current_line_num = m->super.line_num;

	struct Type** expect_types = NULL;
	uint8_t expect_args        = 0;
	
	if(sst_contains(tcctx->st->sst, m->name)){
		
		struct SSTLine* line = sst_get(tcctx->st->sst, m->name);
		
		if( (!tcctx->current_fn->decl->has_side_effects)
			&& line->has_side_effect
		){
			//method with side effects called
			//in method marked as without side effects
			error(tcctx, ERR_CALLED_SIDE_EFFECT_IN_PURE, TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
            return false;
		}

		if(line->is_libc){
		    //we do not have the AST for libC
		    //subroutines, so we cannot typecheck the call
		    return true;
		}
		
		if(line->method == NULL){

			char msg[150];
			sprintf(msg, "subroutine HAS NO METHOD IN SST: %s\n", m->name);
			strcat(msg, ERR_SUBR_NOT_FOUND);
			error(tcctx, msg, TC_ERR_SUBR_NOT_FOUND);
		}
		
		struct Method* method = line->method;
		
        if(!check_throw_rules(method->decl->throws, tcctx)){return false;}
		
		expect_args = method->decl->count_args;

		if (line->type != NULL) {
			if (line->type->m1 != NULL && line->type->m1->subr_type != NULL){
				expect_args = line->type->m1->subr_type->count_arg_types;
			}
		}
		
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = method->decl->args[i]->type;
		}
		
	}else if(lvst_contains(tcctx->st->lvst, m->name)){
		
		struct LVSTLine* line2 = lvst_get(tcctx->st->lvst, m->name);
		
		struct Type* type = line2->type;
		if(type->m1 == NULL || type->m1->subr_type == NULL){
			error(tcctx, "local variable is not a subroutine in LVST", TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE);
            return false;
		}
		struct SubrType* stype = type->m1->subr_type;
		
		if( (!tcctx->current_fn->decl->has_side_effects)
			&& stype->has_side_effects
		){
			//method with side effects called
			//in method marked as without side effects
			error(tcctx, ERR_CALLED_SIDE_EFFECT_IN_PURE, TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
            return false;
		}
		
        if(!check_throw_rules(stype->throws, tcctx)){return false;}
		
		expect_args  = stype->count_arg_types;
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = stype->arg_types[i];
		}
		
	}else{
		
		error(tcctx, ERR_SUBROUTINE_NOT_IN_SST_LVST, TC_ERR_SUBR_NOT_FOUND);
        return false;
	}
	
	bool err1 = tc_methodcall_args(m, expect_types, expect_args, tcctx);
	
	free(expect_types);

    return err1;
}

static bool check_throw_rules(bool callee_throws, struct TCCtx* tcctx){
	
	if(!callee_throws){ return true; }
	if(tcctx->current_fn->decl->throws){ return true; }
	
	if(tcctx->depth_inside_try_stmt > 0){ return true; }
		
	error(tcctx, "called a throwing subroutine inside a non-throwing subroutine outside any try-block", TC_ERR_CALLED_THROWING_WRONG);
    return false;
}

static bool tc_methodcall_args(
	struct Call* m, 
	struct Type** expect_types,
	uint8_t expect_args, 
	struct TCCtx* tcctx
){
	const uint8_t actual_args = m->count_args;
	
	if(actual_args != expect_args){
		
		char* s1 = str_call(m);
		
		char msg[200];
		sprintf(msg, "\t%s\nexpected: %d args\n", s1, expect_args);
		strcat(msg, ERR_NUM_ARGS);
		
		free(s1);
		
		error(tcctx, msg, TC_ERR_ARG_NUM_MISMATCH);
        return false;
	}
	
	for(uint8_t i = 0; i < expect_args; i++){
	
		struct Type* expect_type = expect_types[i];
		
		struct Expr* actual_expr = m->args[i];
		
        if(!tc_methodcall_arg(m, expect_type, actual_expr, tcctx)){return false;}
	}
    return true;
}

static bool tc_methodcall_arg(
	struct Call* m, 
	struct Type* expect_type,
	struct Expr* actual_expr,
	struct TCCtx* tcctx
){
	
	struct Type* actual_type = infer_type_expr(tcctx->current_filename, tcctx->st, actual_expr);

	if(is_integer_type(expect_type) && is_integer_type(actual_type)){ return true; }
		
	if(!eq_type(expect_type, actual_type)){
		
		char* s1 = str_call(m);
		char* s2 = str_expr(actual_expr);
		
		char* sTypeActual   = str_type(actual_type);
		char* sTypeExpected = str_type(expect_type);
		
		char msg[200];
		sprintf(msg, "\t%s\n%s, (of type %s), but expected type %s\n", s1, s2, sTypeActual, sTypeExpected);
		strcat(msg, ERR_ARG_TYPES);
		
		free(s1);
		free(s2);
		free(sTypeActual);
		free(sTypeExpected);
		
		error(tcctx, msg, TC_ERR_ARG_TYPE_MISMATCH);
        return false;
	}
	
    if(!tc_expr(actual_expr, tcctx)){return false;}

    return true;
}
