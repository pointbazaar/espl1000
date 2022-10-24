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

    if(uot->op == OP_NONE){ return true; }

    //check if the optional unary operator is valid for the type of term

    enum OP op = uot->op;

    if(is_integer_type(type)){
        if(op == OP_MINUS || op == OP_PLUS || op == OP_COMPLEMENT){
            return true;
        }
    }

    if(is_bool_type(type)){
        if(op == OP_NOT){
            return true;
        }
    }

    char* snippet = str_un_op_term(uot);
    error_snippet(tcctx, snippet, TC_ERR_WRONG_OP_UNOP);
    free(snippet);

    return false;
}
