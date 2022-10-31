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

void test_compile_tac_return(){

	status_test_codegen("TAC_RETURN");
	
	const int8_t value = rand()%0xff;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(0, value));
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    for(int i=0;i < 6; i++){
        vmcu_system_step(system);
	}
	
	//returned value should be in r0
	assert(vmcu_system_read_gpr(system, 0) == value);
	
	vmcu_system_dtor(system);
}
