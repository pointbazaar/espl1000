//AST Includes
#include <string.h>
#include "ast/ast.h"

#include "typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "tcctx.h"
#include "util/tc_utils.h"
#include "util/tc_errors.h"

bool tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx){

    if(!tc_term(uot->term, tcctx)){return false;}

    struct Type* type = infer_type_term(tcctx->current_filename, tcctx->st, uot->term);

    if(uot->op == NULL){ return true; }

    //check if the optional unary operator is valid for the type of term

    char* op = uot->op->op;

    if(is_integer_type(type)){
        if(strcmp(op, "-") == 0 || strcmp(op, "+") == 0 || strcmp(op, "~") == 0){
            return true;
        }
    }

    if(is_float_type(type)){
        if(strcmp(op, "-") == 0 || strcmp(op, "+") == 0){
            return true;
        }
    }

    if(is_bool_type(type)){
        if(strcmp(op, "!") == 0){
            return true;
        }
    }

    error(tcctx, "illegal combination of unary operator and term type", TC_ERR_WRONG_OP_UNOP);
    return false;
}
