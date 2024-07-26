#include <stdio.h>

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_nop_x86(struct IBuffer* ibu) {

	nop("TAC_NOP");
}
