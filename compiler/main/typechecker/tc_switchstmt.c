#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "tcctx.h"

bool tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx){

    tcctx->current_line_num = s->super.line_num;

    struct Type* type = infer_type_expr(tcctx->st, s->expr);

    if(!is_primitive_type(type)){

        char* s1 = str_expr(s->expr);

        char snippet[200];
        sprintf(snippet, "switch %s {", s1);

        free(s1);

        error_snippet(tcctx, snippet, TC_ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE);
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

            char* snippet = str_case_stmt(c);
            error_snippet(tcctx, snippet, TC_ERR_SWITCH_CASE_TYPE_MISMATCH);
            free(snippet);

            return false;
        }
    }

    return true;
}