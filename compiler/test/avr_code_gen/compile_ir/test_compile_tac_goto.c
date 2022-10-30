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

void test_compile_tac_goto(){
	
	status_test_codegen("TAC_GOTO");
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACLabel(43));
	tacbuffer_append(buffer, makeTACGoto(43));
    tacbuffer_append(buffer, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 7; i++){
        vmcu_system_step(system);
	}
	
	const uint32_t old_pc = vmcu_system_get_pc(system);
	
	for(int i=0;i < 5; i++){
        vmcu_system_step(system);
	}
	
	//assert that the PC has not changed
	assert(vmcu_system_get_pc(system) == old_pc);
	
	vmcu_system_dtor(system);
}
