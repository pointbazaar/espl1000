#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_compile_tac.h"

void test_compile_tac_const_value(){
	
	status_test_codegen("TAC_CONST_VALUE");
	
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tac0 = makeTACConst(1, fixed_value);
    struct TAC* tac1 = makeTACReturn(1);
    
    tacbuffer_append(buffer, tac0);
    tacbuffer_append(buffer, tac1);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
        
	//check that the value was written to any register
	bool found = false;
	
	for(int i = 0; i < 32; i++){
		
		if(vmcu_system_read_gpr(system, i) == fixed_value) found = true;
	}
	
	assert(found);
	
	vmcu_system_dtor(system);
}
