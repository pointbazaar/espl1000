#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_label(struct TAC* tac, FILE* fout, struct Ctx* ctx){
    //newline for readability
    fprintf(fout, "\n");

    if(tac->kind == TAC_LABEL_INDEXED)
        fprintf(fout, "L%d:\n", tac->label_index);
    
    if(tac->kind == TAC_LABEL_FUNCTION){
		
		//in case of tests, sst may not be filled
		char* function_name = "main"; 
		
		if(tac->dest < sst_size(ctx_tables(ctx)->sst)){
				
			function_name = sst_at(ctx_tables(ctx)->sst, tac->dest)->name;
		}
		
        fprintf(fout, "%s:\n", function_name);
	}
}
