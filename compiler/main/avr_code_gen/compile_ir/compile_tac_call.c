#include <stdio.h>

#include "tac/tac.h"

void compile_tac_call(struct RAT* rat, struct TAC* tac, FILE* fout, struct Ctx* ctx){

    int reg_dest = rat_get_register(rat, tac->dest);
    
    //in case of tests, where SST may not be filled correctly
    char* function_name = "main"; 
    
    if(tac->arg1 < sst_size(ctx_tables(ctx)->sst)){
		function_name = sst_at(ctx_tables(ctx)->sst, tac->arg1)->name;
	}	
	
    fprintf(fout, "call %s\n", function_name);
    
    if(reg_dest != 0)
		fprintf(fout, "mov r%d, r0\n", reg_dest);
    
    struct SST* sst = ctx_tables(ctx)->sst;
    
    
    //for the case of tests on raw TACBuffers
    if(sst_size(sst) == 0)
		return;
    
    //pop the PARAMS off the stack
    char* name = sst_at(sst, tac->arg1)->name;
    
    uint32_t size = sst_args_size_avr(sst, name);
    
    for(uint32_t i=0;i < size; i++){
		fprintf(fout, "pop r16\n");
	}
}
