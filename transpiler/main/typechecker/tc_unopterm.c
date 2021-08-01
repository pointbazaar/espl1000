//AST Includes
#include <string.h>
#include "ast/ast.h"

#include "typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "tcctx.h"
#include "util/tc_utils.h"
#include "util/tc_errors.h"

void tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx){

    tc_term(uot->term, tcctx);

    struct Type* type = infer_type_term(tcctx->current_filename, tcctx->st, uot->term);

    if(uot->op == NULL){ goto pass; }

    //check if the optional unary operator is valid for the type of term

    char* op = uot->op->op;

    if(is_integer_type(type)){
        if(strcmp(op, "-") == 0 || strcmp(op, "+") == 0 || strcmp(op, "~") == 0){
            goto pass;
        }
    }

    if(is_float_type(type)){
        if(strcmp(op, "-") == 0 || strcmp(op, "+") == 0){
            goto pass;
        }
    }

    if(is_bool_type(type)){
        if(strcmp(op, "!") == 0){
            goto pass;
        }
    }

    error(tcctx, "illegal combination of unary operator and term type");

    pass:
    return;
}
