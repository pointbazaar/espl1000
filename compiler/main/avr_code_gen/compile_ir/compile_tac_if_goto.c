#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, FILE* fout){

    const int reg = rat_get_register(rat, tac->arg1);

    fprintf(fout, "tst r%d\n", reg); //test if r%d is zero
    
    fprintf(fout, "brne L%d\n", tac->label_index);
}
