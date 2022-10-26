#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

static void case_tac_op_add(FILE* fout, int dest, int32_t immediate);
static void case_tac_op_sub(FILE* fout, int dest, int32_t immediate);

void compile_tac_binary_op_immediate(struct RAT* rat, struct TAC* tac, FILE* fout){

	if(!rat_has_register(rat, tac->dest)){
		//our temporary does not have a register
		int index = rat_get_free_register(rat, false);
		rat_occupy_register(rat, index, tac->dest);
	}

    int dest = rat_get_register(rat, tac->dest);

    const int32_t immediate = tac->const_value;

    switch(tac->op){

        case TAC_OP_ADD:
            case_tac_op_add(fout, dest, immediate);
            break;

        case TAC_OP_SUB:
            case_tac_op_sub(fout, dest, immediate);
            break;

        case TAC_OP_AND:
            fprintf(fout, "andi r%d, %d\n", dest, immediate);
            break;

        case TAC_OP_OR:
            fprintf(fout, "ori r%d, %d\n", dest, immediate);
            break;
            
		case TAC_OP_SHIFT_LEFT:
			for(int i=0; i < immediate; i++){
				fprintf(fout, "lsl r%d\n", dest);
		    }
			break;
			
		case TAC_OP_SHIFT_RIGHT:
			for(int i=0; i < immediate; i++){
				fprintf(fout, "lsr r%d\n", dest);
		    }
			break;

        default:
            printf("unsupported op in compile_tac_binary_op_immediate\n");
            printf("tac->op = %d\n", tac->op);
            exit(1);
    }
}

static void case_tac_op_add(FILE* fout, int dest, int32_t immediate){
	
	if(immediate == 1){
		fprintf(fout, "inc r%d\n", dest);
	}else{
		//here we negate the constant for subi
		//https://sites.google.com/site/avrasmintro/home/basic-arithmetic
		fprintf(fout, "subi r%d, %d\n", dest, -immediate);
	}
}


static void case_tac_op_sub(FILE* fout, int dest, int32_t immediate){
	
	if(immediate == 1)
		fprintf(fout, "dec r%d\n", dest);
	else
		fprintf(fout, "subi r%d, %d\n", dest, immediate);
}
