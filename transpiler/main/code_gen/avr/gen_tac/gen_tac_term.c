#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_term(struct TACBuffer* buffer, struct Term* t){

    switch(t->kind){
        case 4: tac_call(buffer, t->ptr.m4); break;
        case 5: tac_expr(buffer, t->ptr.m5); break;
        case 6: tac_variable(buffer, t->ptr.m6); break;
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
        default:
            break;
    }
}