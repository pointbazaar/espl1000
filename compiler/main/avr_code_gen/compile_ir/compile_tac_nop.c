#include <stdio.h>

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_nop(struct IBuffer* ibu) {

	avr_nop("TAC_NOP");
}
