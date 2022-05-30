#include <stdio.h>
#include <stdlib.h>

#include "avr_code_gen/tac/tac.h"
#include "avr_code_gen/tacbuffer/tacbuffer.h"

#include "gen_tac.h"

void tac_variable(struct TACBuffer* buffer, struct Variable* v){

    tac_simplevar(buffer, v->simple_var);

    if(v->member_access != NULL){
        printf("member access not implemented for avr_code_gen\n");
        exit(1);
    }
}