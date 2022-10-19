#include <string.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "tcctx.h"
#include "util/tc_utils.h"
#include "util/tc_errors.h"

bool tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx){

    if(!tc_term(uot->term, tcctx)){return false;}

    struct Type* type = infer_type_term(tcctx->st, uot->term);

    if(uot->op == NULL){ return true; }

    //check if the optional unary operator is valid for the type of term

    char* op = uot->op->op;

    if(is_integer_type(type)){
        if(strcmp(op, "-") == 0 || strcmp(op, "+") == 0 || strcmp(op, "~") == 0){
            return true;
        }
    }

    if(is_bool_type(type)){
        if(strcmp(op, "!") == 0){
            return true;
        }
    }

    char* snippet = str_un_op_term(uot);
    error_snippet(tcctx, snippet, TC_ERR_WRONG_OP_UNOP);
    free(snippet);

    return false;
}
