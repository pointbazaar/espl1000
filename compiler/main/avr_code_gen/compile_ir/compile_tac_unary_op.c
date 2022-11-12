#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"


void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

    int reg_src = rat_get_register(rat, tac->arg1);
    
    int reg_dest = rat_get_register(rat, tac->dest);

	mov(reg_dest, reg_src);

    switch (tac->op) {
        case TAC_OP_UNARY_NOT:
        case TAC_OP_UNARY_BITWISE_NEG:
			com(reg_dest);
            break;
        case TAC_OP_UNARY_MINUS:
			neg(reg_dest);
            break;
        default:
            break;
    }
}
