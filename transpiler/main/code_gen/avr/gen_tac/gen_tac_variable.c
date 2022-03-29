#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "gen_tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"

void tac_variable(struct TACBuffer* buffer, struct Variable* v){

    tac_simplevar(buffer, v->simple_var);

    if(v->member_access != NULL){
        printf("member access not implemented for avr\n");
        exit(1);
    }
}