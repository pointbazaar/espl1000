#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"


void compile_tac_load_const_addr(struct RAT* rat, struct TAC* tac, FILE* fout){

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        
        rat_occupy_register(rat, index, tac->dest);
    }

    const int reg_dest = rat_get_register(rat, tac->dest);

    const uint32_t addr = tac->const_value;

    fprintf(fout, "lds r%d, %d\n", reg_dest, addr);
}
