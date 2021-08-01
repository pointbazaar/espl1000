#include <string.h>
#include <stdio.h>

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

static void check_expr_well_formed(struct Type* left, struct Type* right, struct TCCtx* tcctx);

void tc_expr(struct Expr* expr, struct TCCtx* tcctx){

	tcctx->current_line_num = expr->super.line_num;

	tc_unopterm(expr->term1, tcctx);
	
	if(expr->op != NULL){

        tc_unopterm(expr->term2, tcctx);

        struct Type* left  = infer_type_unopterm(tcctx->current_filename, tcctx->st, expr->term1);
        struct Type* right = infer_type_unopterm(tcctx->current_filename, tcctx->st, expr->term2);

        check_expr_well_formed(left, right, tcctx);
	}
}

static void check_expr_well_formed(struct Type* left, struct Type* right, struct TCCtx* tcctx){

    if(is_integer_type(left) && is_integer_type(right)){ return; }

    if(is_integer_type(left) && is_float_type(right)){ return; }
    if(is_integer_type(right) && is_float_type(left)){ return; }

    if(!eq_type(left, right)){

        char* s_left  = str_type(left);
        char* s_right = str_type(right);

        char msg[200];
        sprintf(msg, "types not equal in expression. left: %s, right: %s\n", s_left, s_right);
        error(tcctx,msg);
    }
}