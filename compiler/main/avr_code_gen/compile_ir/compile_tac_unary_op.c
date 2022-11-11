#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"


void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, FILE* fout){

    int reg_src = rat_get_register(rat, tac->arg1);
    
    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);

    switch (tac->op) {
        case TAC_OP_UNARY_NOT:
        case TAC_OP_UNARY_BITWISE_NEG:
            fprintf(fout, "com r%d\n", reg_dest);
            break;
        case TAC_OP_UNARY_MINUS:
            fprintf(fout, "neg r%d\n", reg_dest);
            break;
        default:
            printf("error\n");
            exit(1);
    }
}
