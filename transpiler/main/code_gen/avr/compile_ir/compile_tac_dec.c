#include "stdio.h"

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"

void compile_tac_dec(struct RAT* rat, struct TAC* tac, FILE* fout){

    const int reg = rat_get_register(rat, tac->dest);

    fprintf(fout, "dec r%d\n", reg);
}