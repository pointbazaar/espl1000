#include <stdio.h>
#include <stdlib.h>

#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_term(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx){

    switch(t->kind){
        case 4: tac_call(buffer, t->ptr.m4, ctx); break;
        case 5: tac_expr(buffer, t->ptr.m5, ctx); break;
        case 6: tac_variable(buffer, t->ptr.m6, ctx); break;
        case 8:
            printf("string const currently unsupported\n");
            exit(1);
            break;
        case 11:
            printf("Fatal Error. Lambdas should not exist at this stage.\n");
            exit(1);
            //lambdas should not exist anymore at this stage,
            //having been converted into named functions
        case 12: tac_constvalue(buffer, t->ptr.m12); break;
        case 13: tac_mdirect(buffer, t->ptr.m13); break;
        default:
            break;
    }
}
