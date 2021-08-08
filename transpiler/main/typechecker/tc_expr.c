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

static bool check_expr_well_formed(struct Type* left, struct Type* right, struct TCCtx* tcctx);

bool tc_expr(struct Expr* expr, struct TCCtx* tcctx){

	tcctx->current_line_num = expr->super.line_num;

    if(!tc_unopterm(expr->term1, tcctx)){return false;}
	
	if(expr->op != NULL){

        if(!tc_unopterm(expr->term2, tcctx)){return false;}

        struct Type* left  = infer_type_unopterm(tcctx->st, expr->term1);
        struct Type* right = infer_type_unopterm(tcctx->st, expr->term2);

        return check_expr_well_formed(left, right, tcctx);
	}
    return true;
}

static bool check_expr_well_formed(struct Type* left, struct Type* right, struct TCCtx* tcctx){

    if(is_integer_type(left) && is_integer_type(right)){ return true; }

    if(is_integer_type(left) && is_float_type(right)){ return true; }
    if(is_integer_type(right) && is_float_type(left)){ return true; }

    if(!eq_type(left, right)){

        char* s_left  = str_type(left);
        char* s_right = str_type(right);

        char msg[200];
        sprintf(msg, "types not equal in expression. left: %s, right: %s\n", s_left, s_right);
		free(s_left);
		free(s_right);
        error(tcctx,msg, TC_ERR_BINOP_TYPE_MISMATCH);
        return false;
    }

    return true;
}