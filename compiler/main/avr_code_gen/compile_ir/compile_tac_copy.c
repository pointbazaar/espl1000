#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_copy(struct RAT* rat, struct TAC* tac, FILE* fout) {

    const int reg_src = rat_get_register(rat, tac->arg1);

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);
}
