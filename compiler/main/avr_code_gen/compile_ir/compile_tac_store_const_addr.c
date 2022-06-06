#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_const_addr(struct RAT* rat, struct TAC* tac, FILE* fout){

    const int reg_src = rat_get_register(rat, tac->arg1);

    const uint32_t addr = tac->const_value;

    fprintf(fout, "sts %d, r%d\n", addr, reg_src);
}