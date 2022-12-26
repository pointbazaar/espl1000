#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local_addr(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	//TODO: use a register pair to store the address,
	//a single register is not wide enough.

	const int rdest = rat_get_register(rat, tac->dest);

	char* name = lvst_at(ctx_tables(ctx)->lvst, tac->arg1)->name;

	const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);

	//load Y into rdest and add our offset

	mov(rdest, YL);

	if(offset != 0)
		subi(rdest, -offset);
}
