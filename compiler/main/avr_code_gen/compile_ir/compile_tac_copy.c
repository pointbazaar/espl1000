#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_copy(struct RAT* rat, struct TAC* tac, FILE* fout) {
    //left and right operand should have registers

    if(!rat_has_register(rat, tac->arg1)){
        int index = rat_get_free_register(rat, 0);
        rat->is_occupied[index] = true;
        rat->occupant[index] = tac->arg1;

        //TODO: move our src into that register
    }

    int reg_src = rat_get_register(rat, tac->arg1);

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, 0);
        rat->is_occupied[index] = true;
        rat->occupant[index] = tac->dest;
    }

    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);
}