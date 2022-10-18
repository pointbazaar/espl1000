#include <stdlib.h>
#include <stdio.h>

#include <analyzer/lv/lv_analyzer.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Table Includes
#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typecheck.h"
#include "tcctx.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"


bool tc_mdirect(struct MDirect* mdirect, struct TCCtx* tcctx){
	
	tcctx->current_line_num = mdirect->super.line_num;

    struct Type* type = infer_type_expr(tcctx->st, mdirect->expr);

    if(!is_integer_type(type)){

        char* s1 = str_expr(mdirect->expr);

        char msg[100];
        sprintf(msg, "[%s]", s1);

        free(s1);

        error_snippet(tcctx, msg, TC_ERR_INDEX_NOT_INTEGER_TYPE);

        return false;
    }

    return true;
}
