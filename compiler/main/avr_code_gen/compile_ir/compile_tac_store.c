#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store(struct RAT* rat, struct TAC* tac, FILE* fout) {
	
	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);
	
	//set X 
	fprintf(fout, "mov XL, r%d\n", reg_dest);
	fprintf(fout, "ldi XH, 0\n");
	
	fprintf(fout, "st X, r%d; writeback locals\n", reg_src);
}
