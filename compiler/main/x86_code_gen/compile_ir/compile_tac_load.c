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

	const uint8_t width = tac_load_store_width(tac);

	if (width < 4) {
		// movzx: zero-extend the upper bytes of dest register
		movzx_load_width(reg_dest, reg_src, width, c);
	} else {
		// in case of 32-bit source,
		// a mov should automatically zero out the upper bytes of dest register
		mov_load_width(reg_dest, reg_src, width, c);
	}
}
