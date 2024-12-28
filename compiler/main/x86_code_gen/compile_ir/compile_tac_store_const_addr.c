#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "rat/rat.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_const_addr_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_STORE_CONST_ADDR";
	const int reg_src = rat_get_register(rat, tac_arg1(tac));

	const uint64_t addr = tac_const_value(tac);

	const uint32_t rscratch = rat_scratch_reg(rat);

	mov_const(rscratch, addr, c);
	mov_store(rscratch, reg_src, c);
}
