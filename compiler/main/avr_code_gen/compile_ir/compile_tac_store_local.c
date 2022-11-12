#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"

#include "tac/tac.h"

#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {
	
	char* name = lvst_at(ctx_tables(ctx)->lvst, tac->dest)->name;
    
	size_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);

	const int reg = rat_get_register(rat, tac->arg1);
	
	if(offset == 0){
		stY(reg, "TAC_STORE_LOCAL");
		//st Y, reg; writeback locals
	}else {
		stdY(offset, reg, "TAC_STORE_LOCAL");
		//std Y+offset, reg; writeback locals
	}
}
