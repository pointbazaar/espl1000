#include <stdio.h>

#include "tac/tac.h"

void compile_tac_call(struct RAT* rat, struct TAC* tac, FILE* fout, struct Ctx* ctx){
	
	//check if the temp that the returned value has been
	//assigned to has a register
	if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);
    
    //in case of tests, where SST may not be filled correctly
    char* function_name = "main"; 
    
    if(tac->arg1 < sst_size(ctx_tables(ctx)->sst)){
		function_name = sst_at(ctx_tables(ctx)->sst, tac->arg1)->name;
	}	
	
    fprintf(fout, "call %s\n", function_name);
    
    fprintf(fout, "mov r%d, r0", reg_dest);
}
