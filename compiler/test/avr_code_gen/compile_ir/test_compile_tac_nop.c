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

void test_compile_tac_nop(){
	
	status_test_codegen("TAC_NOP");
	
	//test that register etc. do not change with TAC_NOP
	//this test is also intended for completeness

    struct TACBuffer* b = tacbuffer_ctor();
    int n = 8;
    
    for(int i=0; i < n; i++){
		tacbuffer_append(b, makeTACNop());
	}

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
	
	//get into the nop range after 8 steps
    vmcu_system_step_n(system, 8);
    
    //record register values
	int8_t regs[32];
	for(int i = 0; i < 32; i++){
		regs[i] = vmcu_system_read_gpr(system, i);
	}
	
	uint16_t old_sp = vmcu_system_read_sp(system);
	
	//do some steps
	vmcu_system_step_n(system, 5);
	
	//check that the values are still the same
	for(int i = 0; i < 32; i++){
		assert(regs[i] == vmcu_system_read_gpr(system, i));
	}
	
	assert(old_sp == vmcu_system_read_sp(system));
	
	vmcu_system_dtor(system);
}

