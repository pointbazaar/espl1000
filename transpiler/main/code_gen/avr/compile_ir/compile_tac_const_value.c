#include <stdio.h>
#include <stdbool.h>

#include "code_gen/avr/rat/rat.h"

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"

void compile_tac_const_value(struct RAT* rat, struct TAC* tac, FILE* fout){

    int reg = rat_get_free_register(rat, 16);

    rat->is_occupied[reg] = true;
    rat->occupant[reg] = tac->dest;

    fprintf(fout, "ldi r%d, %d\n", reg, tac->const_value);
}