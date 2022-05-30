#include <stdio.h>

#include "rat/rat.h"
#include "avr_code_gen/tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, FILE* fout){

    if(!rat_has_register(rat, tac->arg1)){
        printf("fatal error, %s has no register in compile_tac_if_goto\n", tac->arg1);
    }

    int reg = rat_get_register(rat, tac->arg1);

    fprintf(fout, "mov r16, r%d\n", reg);
    fprintf(fout, "cpi r16, 0\n");

    fprintf(fout, "brne L%d\n", tac->label_index);
}