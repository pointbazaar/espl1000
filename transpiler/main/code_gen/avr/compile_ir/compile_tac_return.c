#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "code_gen/avr/rat/rat.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"

#include "util/ctx.h"

void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout){
    //destroy the stackframe
    fprintf(fout, "\n");
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