#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout) {

    const int reg_dest = rat_get_register(rat, tac->dest);
    
    char* name = lvst_at(ctx_tables(ctx)->lvst, tac->arg1)->name;
    
    const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);
			
	if(offset == 0)
		fprintf(fout, "ld r%d, Y\n", reg_dest);
	else
		fprintf(fout, "ldd r%d, Y+%d\n", reg_dest, offset);
}
