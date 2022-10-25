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

void test_compile_tac_param(){
	
	status_test_codegen("TAC_PARAM");
	
	//test that the value gets pushed
	//and the stack pointer decrements
	
	const int8_t fixed_value = rand()%0xff;
	
	const uint8_t SPL_ADDR = 0x5d;
	const uint8_t SPH_ADDR = 0x5e;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(0, fixed_value));
    tacbuffer_append(buffer, makeTACParam("t0"));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 5; i++){
        vmcu_system_step(system);
	}
	
	int sp_old = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;
	
	for(int i=0;i < 3; i++){
        vmcu_system_step(system);
	}
	
	int sp = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;
	
	//assert that SP was decremented
	assert(sp == sp_old - 1);
	
	//assert that fixed_value is on the stack
	assert(vmcu_system_read_data(system, sp_old) == fixed_value);
	
	vmcu_system_dtor(system);
}
