#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cg_avr_basic_block.h"
#include "rat.h"

#include "tacbuffer.h"
#include "tac.h"

#include "flags.h"

static void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout){
    //destroy the stackframe
    for(size_t k=0; k < lvst_stack_frame_size_avr(ctx->tables->lvst); k++){
        fprintf(fout, "pop r0 ;destroy frame\n");
    }

    //TODO: put the return value in the appropriate register
    if(!rat_has_register(rat, tac->arg1)){
        printf("fatal error, value to be returned was not in a register\n");
        exit(1);
    }

    int reg = rat_get_register(rat, tac->arg1);
    fprintf(fout, "mov r0, r%d\n", reg);

    fprintf(fout, "ret\n");
}

static void compile_tac_const_value(struct RAT* rat, struct TAC* tac, FILE* fout){

    int reg = rat_get_free_register(rat, 16);

    rat->is_occupied[reg] = true;
    rat->occupant[reg] = tac->dest;

    fprintf(fout, "ldi r%d, %d\n", reg, tac->const_value);
}

static void compile_tac_copy(struct RAT* rat, struct TAC* tac, FILE* fout) {
    //left and right operand should have registers

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
}

static void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, FILE* fout){

    //left and right operand should have registers

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

    char* mnem = "?";

    if(strcmp(tac->op, "+=") == 0) mnem = "add";
    if(strcmp(tac->op, "-=") == 0) mnem = "sub";
    if(strcmp(tac->op, "*=") == 0) mnem = "mul";

    if(strcmp(tac->op, "|=") == 0) mnem = "or";
    if(strcmp(tac->op, "&=") == 0) mnem = "and";

    if(strcmp(mnem, "?") == 0){
        printf("Fatal error: could not compile TAC op %s\n", tac->op);
        exit(1);
    }

    fprintf(fout, "%s r%d, r%d\n", mnem, reg_dest, reg_src);
}

static void compile_tac_goto(struct TAC* tac, FILE* fout){
    fprintf(fout, "rjmp L%d\n", tac->goto_index);
}

static void compile_tac_nop(FILE* fout){
    fprintf(fout, "nop\n");
}

static void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, FILE* fout){

    if(!rat_has_register(rat, tac->arg1)){
        printf("fatal error, %s has no register in compile_tac_if_goto\n", tac->arg1);
    }

    int reg = rat_get_register(rat, tac->arg1);

    fprintf(fout, "cpi r%d, 0", reg);

    fprintf(fout, "brne L%d\n", tac->goto_index);
}

static void compile_tac_call(struct TAC* tac, FILE* fout){
    fprintf(fout, "call %s\n", tac->arg1);
}

static void compile_tac_unsupported(char* which){
    //TODO
    printf("currently unsupported: %s\n", which);
    exit(1);
}

void emit_asm_avr_single_tac(struct RAT* rat, struct TAC *tac, struct Ctx* ctx, FILE *fout) {

    if(ctx->flags->debug){
        printf("emit_asm_avr_single_tac %s\n", tac_tostring(tac));
    }

    if(tac->label_index != TAC_NO_LABEL)
        fprintf(fout, "L%d:\n", tac->label_index);

    switch(tac->kind){
        case TAC_GOTO:        compile_tac_goto(tac, fout); break;
        case TAC_NOP:         compile_tac_nop(fout); break;
        case TAC_BINARY_OP:   compile_tac_binary_op(rat, tac, fout); break;
        case TAC_UNARY_OP:
            //TODO
                              compile_tac_unsupported("TAC_UNARY_OP"); break;
        case TAC_IF_GOTO:     compile_tac_if_goto(rat, tac, fout); break;
        case TAC_DEREF:
            //TODO
                              compile_tac_unsupported("TAC_DEREF\n"); break;
        case TAC_COPY:        compile_tac_copy(rat, tac, fout); break;
        case TAC_CONST_VALUE: compile_tac_const_value(rat, tac, fout); break;
        case TAC_CONST_STRING:
            //TODO
                              compile_tac_unsupported("TAC_CONST_STRING\n"); break;
        case TAC_CALL:        compile_tac_call(tac, fout); break;
        case TAC_PARAM:
            //TODO
                              compile_tac_unsupported("TAC_PARAM\n"); break;
        case TAC_RETURN:      compile_tac_return(rat, tac, ctx, fout); break;
    }
}