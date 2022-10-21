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

    struct TACBuffer* buffer = tacbuffer_ctor();
    int n = 8;

    struct TAC* tac0 = makeTACConst(1, 0x00);
    struct TAC* tac2 = makeTACReturn("t1");

    tacbuffer_append(buffer, tac0);
    
    for(int i=0; i < n; i++){
		tacbuffer_append(buffer, makeTACNop());
	}
    
    tacbuffer_append(buffer, tac2);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
	
	//get into the nop range after 8 steps
    for(int i=0;i < 8; i++){
        vmcu_system_step(system);
	}
    
    //record register values
	int8_t regs[32];
	for(int i = 0; i < 32; i++){
		regs[i] = vmcu_system_read_gpr(system, i);
	}
	
	//do some steps
	for(int i=0;i < 5; i++){
        vmcu_system_step(system);
	}
	
	//check that the values are still the same
	for(int i = 0; i < 32; i++){
		assert(regs[i] == vmcu_system_read_gpr(system, i));
	}
	
	vmcu_system_dtor(system);
}

