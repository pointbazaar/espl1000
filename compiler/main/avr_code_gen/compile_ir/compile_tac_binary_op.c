#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, FILE* fout){

    //left and right operand should have registers

    if(!rat_has_register(rat, tac->arg1)){
        int index = rat_get_free_register(rat, 0);
        rat_occupy_register(rat, index, tac->arg1);

        //TODO: move our src into that register
    }

    int reg_src = rat_get_register(rat, tac->arg1);

    if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, 0);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);

    char* mnem = "?";

    switch (tac->op) {

        case TAC_OP_NONE:
            printf("tac->op == TAC_OP_NONE\n");
            break;

        case TAC_OP_ADD: mnem = "add"; break;
        case TAC_OP_SUB: mnem = "sub"; break;
        case TAC_OP_MUL: mnem = "mul"; break;
        case TAC_OP_AND: mnem = "and"; break;
        case TAC_OP_OR:  mnem = "or";  break;


        case TAC_OP_CMP_LT: mnem = "brlt"; break;
        case TAC_OP_CMP_GE: mnem = "brge"; break;
        case TAC_OP_CMP_EQ: mnem = "breq"; break;
        case TAC_OP_CMP_NEQ: mnem = "brne"; break;

        case TAC_OP_UNARY_MINUS:
        case TAC_OP_UNARY_NOT:
        case TAC_OP_UNARY_BITWISE_NEG:
            printf("TAC_OP_... currently unsupported\n");
            break;
    }

    if(tac->op >= TAC_OP_CMP_LT && tac->op <= TAC_OP_CMP_NEQ){
        char Ltrue[20];
        char Lend[20];

        static int label_counter = 0;
        sprintf(Ltrue, "Ltrue%d", label_counter);
        sprintf(Lend, "Lend%d", label_counter++);

        fprintf(fout, "cp r%d, r%d\n", reg_dest, reg_src);
        fprintf(fout, "%s %s\n", mnem, Ltrue);

        fprintf(fout, "clr r%d\n", reg_dest);

        fprintf(fout, "rjmp %s\n", Lend);
        fprintf(fout, "%s:\n", Ltrue);

        fprintf(fout, "clr r%d\n", reg_dest);
        fprintf(fout, "inc r%d\n", reg_dest);

        fprintf(fout, "%s:\n", Lend);

        //cp r1,r2
        //brlt Ltrue
        //r1 = 0
        //goto Lend
        //Ltrue:
        //r1 = 1
        //Lend:

    }else {
        fprintf(fout, "%s r%d, r%d\n", mnem, reg_dest, reg_src);
    }
}
