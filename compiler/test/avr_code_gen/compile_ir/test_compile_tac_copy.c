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

void test_compile_tac_copy(){

	status_test_codegen("TAC_COPY");
	
	const int8_t fixed_value = rand()%0xff;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(0, fixed_value));
    tacbuffer_append(buffer, makeTACCopy(1,0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
	
	size_t count = 0;
	
	for(int k = 0; k < 32; k++){
		
		if(vmcu_system_read_gpr(system, k) == fixed_value) count++;
	}
	
	assert(count >= 2);
	
	vmcu_system_dtor(system);
}
