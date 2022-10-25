#include <stdlib.h>
#include <stdio.h>

#include "cg_avr_basic_block.h"

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

#include "../../cli/flags/flags.h"

void emit_asm_avr_single_tac(struct RAT* rat, struct TAC *tac, struct Ctx* ctx, FILE *fout) {

    if(flags_debug(ctx_flags(ctx))){
        printf("emit_asm_avr_single_tac %s\n", tac_tostring(tac));
    }

    switch(tac->kind){
        case TAC_LABEL:       compile_tac_label(tac, fout); break;
        case TAC_GOTO:        compile_tac_goto(tac, fout); break;
        case TAC_NOP:         compile_tac_nop(fout); break;
        case TAC_BINARY_OP:   compile_tac_binary_op(rat, tac, fout); break;
        case TAC_UNARY_OP:    compile_tac_unary_op(rat, tac, fout); break;
        case TAC_IF_GOTO:     compile_tac_if_goto(rat, tac, fout); break;
        case TAC_COPY:        compile_tac_copy(rat, tac, ctx, fout); break;
        case TAC_CONST_VALUE: compile_tac_const_value(rat, tac, fout); break;
        case TAC_CALL:        compile_tac_call(tac, fout); break;
        case TAC_PARAM:		  compile_tac_param(rat, tac, fout); break;
        case TAC_RETURN:      compile_tac_return(rat, tac, ctx, fout); break;
        case TAC_BINARY_OP_IMMEDIATE: compile_tac_binary_op_immediate(rat, tac, fout); break;
        case TAC_LOAD_CONST_ADDR: compile_tac_load_const_addr(rat, tac, fout); break;
        case TAC_STORE_CONST_ADDR: compile_tac_store_const_addr(rat, tac, fout); break;
        case TAC_SETUP_STACKFRAME: compile_tac_setup_stackframe(tac, fout); break;
    }
}
