#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_STORE";

	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);

	mov_store(reg_dest, reg_src, c);
}
