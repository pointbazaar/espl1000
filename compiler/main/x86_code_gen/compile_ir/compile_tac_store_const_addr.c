#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_const_addr_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_src = rat_get_register(rat, tac->arg1);

	const uint32_t addr = tac->const_value;

	mov_store(addr, reg_src, "TAC_STORE_CONST_ADDR");
}
