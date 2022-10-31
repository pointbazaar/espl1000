#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load(struct RAT* rat, struct TAC* tac, FILE* fout) {
    
    //t1 = [t2]
    
    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    const int reg_dest = rat_get_register(rat, tac->dest);
    
    const int reg_src = rat_get_register(rat, tac->arg1);
    
    //TODO: set X to t2
    fprintf(fout, "mov XL, r%d\n", reg_src);
    fprintf(fout, "ldi XH, 0\n");
			
	fprintf(fout, "ld r%d, X\n", reg_dest);
}
