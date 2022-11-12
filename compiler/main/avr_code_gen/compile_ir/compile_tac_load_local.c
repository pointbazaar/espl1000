#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

    const int reg_dest = rat_get_register(rat, tac->dest);
    
    char* name = lvst_at(ctx_tables(ctx)->lvst, tac->arg1)->name;
    
    const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);
			
	if(offset == 0)
		//ld reg_dest, Y
		ldY(reg_dest, "TAC_LOAD_LOCAL");
	else
		lddY(reg_dest, offset, "TAC_LOAD_LOCAL");
		//ldd reg_dest, Y+offset
}
