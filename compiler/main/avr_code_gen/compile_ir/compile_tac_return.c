#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"

void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout){
    //destroy the stackframe
    fprintf(fout, "\n");
    for(size_t k=0; k < lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst); k++){
        fprintf(fout, "pop r0 ;destroy frame\n");
    }

    int reg = rat_get_register(rat, tac->dest);
    fprintf(fout, "mov r0, r%d\n", reg);

    fprintf(fout, "ret\n");
}
