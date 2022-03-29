#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u){

    tac_term(buffer, u->term);

    if(u->op != NULL){
        struct TAC* t = makeTAC();
        t->kind = TAC_UNARY_OP;
        sprintf(t->dest,"t%d", make_temp());
        strcpy(t->arg1, buffer->buffer[buffer->count - 1]->dest);

        t->op = TAC_OP_NONE;
        char* opstr = u->op->op;

        if(strcmp(opstr, "+") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "-") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "*") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "/") == 0) t->op = TAC_OP_ADD;

        if(t->op == TAC_OP_NONE){
            printf("error, t->op was none of supported TAC_OP_... values\n");
            exit(1);
        }

        tacbuffer_append(buffer, t, true);
    }
}