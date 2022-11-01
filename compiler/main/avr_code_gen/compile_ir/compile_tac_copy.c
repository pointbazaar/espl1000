#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_copy(struct RAT* rat, struct TAC* tac, FILE* fout) {

    const int reg_src = rat_get_register(rat, tac->arg1);

    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);
}
