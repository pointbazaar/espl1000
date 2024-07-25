#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"

void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_RETURN";

	//destroy the stackframe
	for (size_t k = 0; k < lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst); k++) {

		pop(0, "destroy frame");
		//pop r0 ; destroy frame
	}

	//find out if tac->dest is a wide (16 bit) temporary

	int reg = rat_get_register(rat, tac->dest);

	mov(0, reg, c);

	if (rat_is_wide(rat, tac->dest)) {
		mov(1, reg + 1, c);
	}

	ret(c);
}
