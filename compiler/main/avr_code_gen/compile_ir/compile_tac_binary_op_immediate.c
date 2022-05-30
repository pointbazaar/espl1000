#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_binary_op_immediate(struct RAT* rat, struct TAC* tac, FILE* fout){

    int dest = rat_get_register(rat, tac->dest);

    const int32_t immediate = tac->const_value;

    switch(tac->op){

        case TAC_OP_ADD:
            if(immediate == 1){
                fprintf(fout, "inc r%d\n", dest);
            }else{
                printf("fatal error in compile_tac_binary_op_immediate\n");
                exit(1);
            }
            break;

        case TAC_OP_SUB:
            if(immediate == 1)
                fprintf(fout, "dec r%d\n", dest);
            else
                fprintf(fout, "subi r%d, $%d\n", dest, immediate);
            break;

        case TAC_OP_AND:
            fprintf(fout, "andi r%d, $%d\n", dest, immediate);
            break;

        case TAC_OP_OR:
            fprintf(fout, "ori r%d, $%d\n", dest, immediate);
            break;

        default:
            printf("unsupported op in compile_tac_binary_op_immediate\n");
            printf("tac->op = %d\n", tac->op);
            exit(1);
    }
}