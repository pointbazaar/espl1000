//AST Includes
#include <stdio.h>
#include "ast/ast.h"
#include "tables/symtable/symtable.h"

#include "tables/lvst/lvst.h"
#include "_tc.h"

#include "typeinference/typeinfer.h"
#include "tcctx.h"

#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"

bool tc_var(struct Variable* v, struct TCCtx* tcctx){

    struct SimpleVar* sv = v->simple_var;

    if(!tc_simplevar(sv, tcctx)){return false;}

    struct Variable* member_access = v->member_access;

    if(member_access != NULL){
        //TODO: check that access is be a member that actually exists on that structure

        //tc_var(member_access, tcctx);
    }
    return true;
}

bool tc_simplevar(struct SimpleVar* sv, struct TCCtx* tcctx){

    //does it even exist in the symbol table?
    char* name = sv->name;

    bool in_lvst = lvst_contains(tcctx->st->lvst, name);
    bool in_sst  = sst_contains(tcctx->st->sst, name);

    if(!in_lvst && !in_sst){

        char msg[100];
        sprintf(msg,"simplevar '%s' not found in lvst/sst", name);
        error(tcctx, msg, TC_ERR_VAR_NOT_FOUND);
        return false;
    }

    if(!in_lvst && sv->count_indices > 0){
        error(tcctx, "cannot use indices for something thats not a local var/arg", TC_ERR_TOO_MANY_INDICES);
        return false;
    }

    //check that each index is of an integer type
    for(uint32_t  i = 0; i < sv->count_indices; i++){

        struct Expr* indexExpr = sv->indices[i];
        struct Type* type = infer_type_expr(tcctx->current_filename, tcctx->st, indexExpr);

        if(!is_integer_type(type)){

            error(tcctx, "index is not an integer type", TC_ERR_INDEX_NOT_INTEGER_TYPE);
            return false;
        }

        if(!tc_expr(indexExpr, tcctx)){return false;}
    }

    if(sv->count_indices == 0){ return true; }

    //check that the correct number of indices was used
    //(meaning not too many)
    struct LVSTLine* line = lvst_get(tcctx->st->lvst, name);

    uint32_t max_indices = max_indices_allowed(line->type);

    if(sv->count_indices > max_indices){
        error(tcctx, "used more indices than this simplevar had", TC_ERR_TOO_MANY_INDICES);
        return false;
    }

    return true;
}