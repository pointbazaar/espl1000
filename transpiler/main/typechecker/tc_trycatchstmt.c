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
#include "tcctx.h"

bool tc_trycatchstmt(struct TryCatchStmt* tcs, struct TCCtx* tcctx){

    tcctx->current_line_num = tcs->super.line_num;

    tcctx->depth_inside_try_stmt++;
    bool is_ok = tc_stmtblock(tcs->try_block, tcctx);
    tcctx->depth_inside_try_stmt--;

    if(!is_ok){return false;}

    return tc_stmtblock(tcs->catch_block, tcctx);
}
