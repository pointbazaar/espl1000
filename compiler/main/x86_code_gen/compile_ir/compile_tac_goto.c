#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_goto(struct TAC* tac, struct IBuffer* ibu) {

	char str[32];
	sprintf(str, "L%d", tac->label_index);

	rjmp(str, "TAC_GOTO");
}
