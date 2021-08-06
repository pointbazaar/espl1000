#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

struct Range;

bool tc_range(struct Range* r, struct TCCtx* tcctx){

    tcctx->current_line_num = r->super.line_num;

    struct Type* t1 = infer_type_expr(tcctx->st, r->start);
    struct Type* t2 = infer_type_expr(tcctx->st, r->end);

    if(!is_integer_type(t1) || !is_integer_type(t2)){

        char* sRange = str_range(r);

        char msg[200];
        sprintf(msg, "\t%s\n", sRange);
        strcat(msg, ERR_RANGE_REQUIRES_INT);

        free(sRange);

        error(tcctx, msg, TC_ERR_RANGE_REQUIRES_INT);
        return false;
    }

    return true;
}