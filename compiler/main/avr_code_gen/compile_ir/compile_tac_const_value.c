#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_value(struct RAT* rat, struct TAC* tac, FILE* fout){

	//for TAC_CONST_VALUE there should have been a higher
	//reg allocated for it's destination tmp
    
    int reg = rat_get_register(rat, tac->dest);
    
    int16_t value = tac->const_value;
    
    if(rat_is_wide(rat, tac->dest)){
		
		fprintf(fout, "ldi r%d, %d\n", reg,   value & 0xff);
		fprintf(fout, "ldi r%d, %d\n", reg+1, (value >> 8) & 0xff);
		
	}else{
		
		fprintf(fout, "ldi r%d, %d\n", reg, value & 0xff);
	}
}
