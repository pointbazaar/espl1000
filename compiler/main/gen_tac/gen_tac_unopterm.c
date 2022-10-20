#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u){

    tac_term(buffer, u->term);

    if(u->op != NULL){
        
        char dest_str[10];
        sprintf(dest_str, "t%d", make_temp());

        enum TAC_OP op = TAC_OP_NONE;
        char* opstr = u->op->op;

        if(strcmp(opstr, "-") == 0) op = TAC_OP_UNARY_MINUS;
        if(strcmp(opstr, "!") == 0) op = TAC_OP_UNARY_NOT;
        if(strcmp(opstr, "~") == 0) op = TAC_OP_UNARY_BITWISE_NEG;

        if(op == TAC_OP_NONE){
            printf("error, op was none of supported TAC_OP_... values\n");
            printf("opstr is %s\n", opstr);
            exit(1);
        }
        
        struct TAC* t = makeTACUnaryOp(dest_str, tacbuffer_last_dest(buffer), op);

        tacbuffer_append(buffer, t);
    }
}
