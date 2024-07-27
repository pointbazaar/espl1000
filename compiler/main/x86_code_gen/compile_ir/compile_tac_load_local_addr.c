#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local_addr_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_LOAD_LOCAL_ADDR";

	const int rdest = rat_get_register(rat, tac->dest);

	char* name = lvst_at(ctx_tables(ctx)->lvst, tac->arg1)->name;

	const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);

	//load Y into rdest and add our offset

	//mov(rdest, YL, c);
	//mov(rdest + 1, YH, c);

	if (offset != 0) {
		//ldi(RAT_SCRATCH_REG, 0, "TAC_LOAD_LOCAL_ADDR"); //OLD

		int16_t o2 = -offset;

		//subi(rdest, -offset); //OLD
		//sbc(rdest+1, RAT_SCRATCH_REG, "TAC_LOAD_LOCAL_ADDR"); //OLD

		//subi(rdest, o2 & 0xff, c);
		//sbci(rdest + 1, (o2 & 0xff00) >> 8, c);
	}
}
