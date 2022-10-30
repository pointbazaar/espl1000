#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

    tac_simplevar(buffer, v->simple_var, ctx);

    if(v->member_access != NULL){
        printf("member access not implemented for avr_code_gen\n");
        exit(1);
    }
}
