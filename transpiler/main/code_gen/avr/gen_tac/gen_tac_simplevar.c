#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "gen_tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v){

    if(v->count_indices != 0){
        //TODO
        printf("indices not yet implemented for avr\n");
        exit(1);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    sprintf(t->dest, "t%d", make_temp());
    strcpy(t->arg1, v->name);

    tacbuffer_append(buffer, t, true);
}