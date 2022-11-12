#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

static void case_tac_op_add(struct IBuffer* ibu, int dest, int32_t immediate);
static void case_tac_op_sub(struct IBuffer* ibu, int dest, int32_t immediate);

void compile_tac_binary_op_immediate(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

    int dest = rat_get_register(rat, tac->dest);

    const int32_t immediate = tac->const_value;

    switch(tac->op){

        case TAC_OP_ADD:
            case_tac_op_add(ibu, dest, immediate);
            break;

        case TAC_OP_SUB:
            case_tac_op_sub(ibu, dest, immediate);
            break;

        case TAC_OP_AND:
			andi(dest, immediate);
            break;

        case TAC_OP_OR:
			ori(dest, immediate);
            break;
            
		case TAC_OP_SHIFT_LEFT:
			for(int i=0; i < immediate; i++){
				lsl(dest);
		    }
			break;
			
		case TAC_OP_SHIFT_RIGHT:
			for(int i=0; i < immediate; i++){
				lsr(dest);
		    }
			break;

        default:
            printf("unsupported op in compile_tac_binary_op_immediate\n");
            printf("tac->op = %d\n", tac->op);
            exit(1);
    }
}

static void case_tac_op_add(struct IBuffer* ibu, int dest, int32_t immediate){
	
	if(immediate == 1){
		inc(dest);
	}else{
		//here we negate the constant for subi
		//https://sites.google.com/site/avrasmintro/home/basic-arithmetic
		
		//find other solution if dest is < r16
		if(dest >= 16){
			subi(dest, -immediate);
		}else{
			//cannot use subi
			ldi(RAT_SCRATCH_REG, -immediate, "");
			sub(dest, RAT_SCRATCH_REG);
		}
	}
}


static void case_tac_op_sub(struct IBuffer* ibu, int dest, int32_t immediate){
	
	if(immediate == 1){
		dec(dest);
	}else{
		
		//find other solution if dest is < r16
		if(dest >= 16){
			subi(dest, immediate);
		}else{
			//cannot use subi
			ldi(RAT_SCRATCH_REG, immediate, "");
			sub(dest, RAT_SCRATCH_REG);
		}
	}
}
