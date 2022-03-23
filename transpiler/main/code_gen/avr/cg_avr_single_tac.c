
#include "cg_avr_basic_block.h"
#include "rat.h"
#include "basicblock.h"
#include "tacbuffer.h"
#include "tac.h"
#include "flags.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void emit_asm_avr_single_tac(struct RAT* rat, struct TAC *tac, struct Flags *flags, FILE *fout) {

    if(flags->debug){
        printf("emit_asm_avr_single_tac %s\n", tac_tostring(tac));
    }

    if(tac->label_index != TAC_NO_LABEL)
        fprintf(fout, "L%d:\n", tac->label_index);

    switch(tac->kind){
        case TAC_GOTO:
            fprintf(fout, "rjmp L%d\n", tac->goto_index);
            break;
        case TAC_NONE:
            break;
        case TAC_NOP:
            fprintf(fout, "nop\n");
            break;
        case TAC_BINARY_OP:
            //TODO
            break;
        case TAC_UNARY_OP:
            //TODO
            break;
        case TAC_IF_GOTO:
            //TODO: check the condition
            fprintf(fout, "rjmp L%d\n", tac->goto_index);
            break;
        case TAC_DEREF:
            //TODO
            printf("currently unsupported: TAC_DEREF\n");
            exit(1);
            break;
        case TAC_COPY:
            //left and right operand should have registers
            {}

            if(!rat_has_register(rat, tac->arg1)){
                int index = rat_get_free_register(rat, 0);
                rat->is_occupied[index] = true;
                rat->occupant[index] = tac->arg1;

                //TODO: move our src into that register
            }

            int reg_src = rat_get_register(rat, tac->arg1);

            if(!rat_has_register(rat, tac->dest)){
                int index = rat_get_free_register(rat, 0);
                rat->is_occupied[index] = true;
                rat->occupant[index] = tac->dest;
            }

            int reg_dest = rat_get_register(rat, tac->dest);

            fprintf(fout, "mov r%d, r%d\n", reg_dest, reg_src);
            break;
        case TAC_CONST_VALUE:
            {}
            int reg = rat_get_free_register(rat, 16);
            rat->is_occupied[reg] = true;
            rat->occupant[reg] = "const";

            fprintf(fout, "ldi r%d, %d\n", reg, tac->const_value);
            break;
        case TAC_CONST_STRING:
            //TODO
            printf("currently unsupported: TAC_CONST_STRING\n");
            exit(1);
            break;
        case TAC_CALL:
            fprintf(fout, "call %s\n", tac->arg1);
            break;
        case TAC_PARAM:
            //TODO
            printf("currently unsupported: TAC_PARAM\n");
            exit(1);
            break;
        case TAC_RETURN:
            fprintf(fout, "ret\n");
            break;
    }
}