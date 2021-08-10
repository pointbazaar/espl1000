#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <typechecker/type_contains/tc_type_contains.h>

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
	
	if(sst_contains(tcctx->st->sst, m->callable->simple_var->name)){
		
		struct SSTLine* line = sst_get(tcctx->st->sst, m->callable->simple_var->name);
		
		if( (!tcctx->current_fn->decl->has_side_effects)
			&& line->has_side_effect
		){
			//method with side effects called
			//in method marked as without side effects
			error(tcctx, "called subr with side-effects in pure subr", TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
            return false;
		}

		if(line->is_libc){
		    //we do not have the AST for libC
		    //subroutines, so we cannot typecheck the call
		    return true;
		}
		
		if(line->method == NULL){

			char msg[150];
			sprintf(msg, "subroutine HAS NO METHOD IN SST: %s", m->callable->simple_var->name);
			
			error(tcctx, msg, TC_ERR_SUBR_NOT_FOUND);
		}
		
		struct Method* method = line->method;
		
        if(!check_throw_rules(method->decl->throws, tcctx)){return false;}
		
		expect_args = method->decl->count_args;

		if (line->type != NULL) {
			if (line->type->basic_type != NULL && line->type->basic_type->subr_type != NULL){
				expect_args = line->type->basic_type->subr_type->count_arg_types;
			}
		}
		
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = method->decl->args[i]->type;
		}
		
	}else if(lvst_contains(tcctx->st->lvst, m->callable->simple_var->name)){
		
		struct LVSTLine* line2 = lvst_get(tcctx->st->lvst, m->callable->simple_var->name);
		
		struct Type* type = line2->type;
		if(type->basic_type == NULL || type->basic_type->subr_type == NULL){

		    char* snippet = str_call(m);
			error_snippet(tcctx, snippet, TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE);
			free(snippet);

            return false;
		}
		struct SubrType* stype = type->basic_type->subr_type;
		
		if( (!tcctx->current_fn->decl->has_side_effects)
			&& stype->has_side_effects
		){
			//method with side effects called
			//in method marked as without side effects
			error(tcctx, "called subr with side effects in pure subr", TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
            return false;
		}
		
        if(!check_throw_rules(stype->throws, tcctx)){return false;}
		
		expect_args  = stype->count_arg_types;
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = stype->arg_types[i];
		}
		
	}else{
		
		char* snippet = str_call(m);
		
		error_snippet(tcctx, snippet, TC_ERR_SUBR_NOT_FOUND);
		
		free(snippet);
		
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
		
		char* snippet = str_call(m);
		
		char msg[200];
		sprintf(msg, "expected: %d args", expect_args);

        error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_ARG_NUM_MISMATCH);

        free(snippet);
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
	
	struct Type* actual_type = infer_type_expr(tcctx->st, actual_expr);

	//TODO: we can get rid of the integer special cases maybe
	//thansk to tc_type_contains
	if(is_integer_type(expect_type) && is_integer_type(actual_type)){ return true; }

	//TODO: deal with the type parameter binding,
	//meaning calling list_add(List<?T0> list, ?T0 elem)
	//with list of type List<int> must bind ?T0 := int
	//and typecheck elem to be of type int.
	if(! tc_type_contains(expect_type, actual_type)){
		
		char* snippet = str_call(m);
		char* s2 = str_expr(actual_expr);
		
		char* sTypeActual   = str_type(actual_type);
		char* sTypeExpected = str_type(expect_type);
		
		char msg[200];
		sprintf(msg, "%s, (of type %s), but expected type %s", s2, sTypeActual, sTypeExpected);
		
		free(s2);
        free(sTypeActual);
        free(sTypeExpected);

        error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_ARG_TYPE_MISMATCH);

        free(snippet);

        return false;
	}
	
    if(!tc_expr(actual_expr, tcctx)){return false;}

    return true;
}
