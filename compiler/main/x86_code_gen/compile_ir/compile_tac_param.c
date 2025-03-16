#include <stdbool.h>

#include "rat/rat.h"
#include "rat/rat_x86.h"

#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_param_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const char* c = "TAC_PARAM";

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	const bool is_syscall = tac_param_is_syscall(tac);

	const enum SD_REGISTER param_reg = rat_param_reg_x86(tac_param_index(tac), is_syscall);

	mov_regs(param_reg, reg_dest, c);
}
