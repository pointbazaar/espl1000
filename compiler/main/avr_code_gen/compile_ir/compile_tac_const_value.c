#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_value(struct RAT* rat, struct TAC* tac, FILE* fout){

    int reg = rat_get_free_register(rat, 16);

	rat_occupy_register(rat, reg, tac->dest);

    fprintf(fout, "ldi r%d, %d\n", reg, tac->const_value);
}
