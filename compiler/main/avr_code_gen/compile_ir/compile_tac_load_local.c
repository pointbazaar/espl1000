#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout) {
    
    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    const int reg_dest = rat_get_register(rat, tac->dest);
    
    const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, tac->arg1);
			
	if(offset == 0)
		fprintf(fout, "ld r%d, Y\n", reg_dest);
	else
		fprintf(fout, "ldd r%d, Y+%d\n", reg_dest, offset);
}
