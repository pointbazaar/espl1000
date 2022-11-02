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
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(0, fixed_value));
    tacbuffer_append(b, makeTACCopy(1,0));
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	size_t count = 0;
	
	for(int k = 0; k < 32; k++){
		
		if(vmcu_system_read_gpr(system, k) == fixed_value) count++;
	}
	
	assert(count >= 2);
	
	vmcu_system_dtor(system);
}
