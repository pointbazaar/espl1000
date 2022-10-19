#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/equals_ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "util/tc_errors.h"
#include "util/tc_utils.h"
#include "tcctx.h"

static bool check_expr_well_formed(struct Type* left, struct Type* right);

bool tc_expr(struct Expr* expr, struct TCCtx* tcctx){

	tcctx->current_line_num = expr->super.line_num;

    if(!tc_unopterm(expr->term1, tcctx)){return false;}
	
	if(expr->op != NULL){

        if(!tc_unopterm(expr->term2, tcctx)){return false;}

        struct Type* left  = infer_type_unopterm(tcctx->st, expr->term1);
        struct Type* right = infer_type_unopterm(tcctx->st, expr->term2);

        bool well_formed = check_expr_well_formed(left, right);
        
        if(!well_formed){
		
			char* snippet = str_expr(expr);
			
			char* s_left  = str_type(left);
			char* s_right = str_type(right);
			
			char msg[200];
			sprintf(msg, "c_types_util not equal in expression. left: %s, right: %s", s_left, s_right);
			
			error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_BINOP_TYPE_MISMATCH);
		
			free(snippet);
			free(s_left);
			free(s_right);
		}
        
        return well_formed;
	}
    return true;
}

static bool check_expr_well_formed(struct Type* left, struct Type* right){

    if(is_integer_type(left) && is_integer_type(right)){ return true; }

    if(!eq_type(left, right)){

        return false;
    }

    return true;
}
