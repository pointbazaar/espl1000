#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout) {
	
	char* name = lvst_get_2(ctx_tables(ctx)->lvst, tac->dest)->name;
    
	size_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);

	const int reg = rat_get_register(rat, tac->arg1);
	
	if(offset == 0){
		fprintf(fout, "st Y, r%d; writeback locals\n", reg);
	}else {
		fprintf(fout, "std Y+%zu, r%d; writeback locals\n", offset, reg);
	}
}
