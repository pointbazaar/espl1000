#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_param(struct RAT* rat, struct TAC* tac, FILE* fout){

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, 0);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "push r%d\n", reg_dest);
}
