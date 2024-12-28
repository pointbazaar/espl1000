#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_LOAD";

	//tdest = [targ1]

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	const int reg_src = rat_get_register(rat, tac_arg1(tac));

	mov_load(reg_dest, reg_src, c);
}
