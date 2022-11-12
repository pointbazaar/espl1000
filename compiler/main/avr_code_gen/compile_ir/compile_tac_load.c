#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {
    
    //t1 = [t2]
    
    const int reg_dest = rat_get_register(rat, tac->dest);
    
    const int reg_src = rat_get_register(rat, tac->arg1);
    
    mov(XL, reg_src);
    ldi(XH, 0, "TAC_LOAD");
    
	ldX(reg_dest, "TAC_LOAD"); //ld reg_dest, X
}
