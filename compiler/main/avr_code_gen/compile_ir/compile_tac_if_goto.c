#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

    const int reg = rat_get_register(rat, tac->arg1);
    char str[32];
    sprintf(str, "L%d", tac->label_index);

    tst(reg); //test if r%d is zero
    brne(str);
}
