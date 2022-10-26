#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_copy(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout) {
    //left and right operand should have registers

    if(!rat_has_register(rat, tac->arg1)){
        int index = rat_get_free_register(rat, false);
        
        //if it's a local var, load it from the stack frame
        //into this register
        struct LVST* lvst = ctx_tables(ctx)->lvst;
        if(lvst_contains(lvst, tac->arg1)){
			uint16_t offset = lvst_stack_frame_offset_avr(lvst, tac->arg1);
			
			if(offset == 0)
				fprintf(fout, "ld r%d, Y\n", index);
			else
				fprintf(fout, "ldd r%d, Y+%d\n", index, offset);
		}
        
		rat_occupy_register(rat, index, tac->arg1);
    }

    int reg_src = rat_get_register(rat, tac->arg1);

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);

    fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);
}
