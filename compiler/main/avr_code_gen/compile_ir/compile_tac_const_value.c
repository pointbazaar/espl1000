#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_value(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

	//for TAC_CONST_VALUE there should have been a higher
	//reg allocated for it's destination tmp
    
    int reg = rat_get_register(rat, tac->dest);
    
    int16_t value = tac->const_value;
    
    if(rat_is_wide(rat, tac->dest)){
		
		ldi(reg,   value & 0xff, "TAC_CONST_VALUE");
		ldi(reg+1, (value >> 8) & 0xff, "TAC_CONST_VALUE");
		
	}else{
		
		ldi(reg, value & 0xff, "TAC_CONST_VALUE");
	}
}
