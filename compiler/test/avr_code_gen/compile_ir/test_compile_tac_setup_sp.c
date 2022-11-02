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

void test_compile_tac_setup_sp(){
	
	//see if stackpointer is setup correctly
	status_test_codegen("TAC_SETUP_SP");
	
	status_test_codegen("stackpointer setup correctly");

    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACSetupSP());
    tacbuffer_append(buffer, makeTACNop());

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
    
    //record register values
	int8_t regs[32];
	for(int i = 0; i < 32; i++){
		if(i == 16) continue;
		regs[i] = vmcu_system_read_gpr(system, i);
	}

    vmcu_system_step_n(system, 4);

    uint16_t sp = vmcu_system_read_sp(system);

	//check that SP was setup
    assert(sp == 0x085f);
    
	for(int i = 0; i < 32; i++){
		if(i == 16) continue;
		assert(regs[i] == vmcu_system_read_gpr(system, i));
	}
	
	vmcu_system_dtor(system);
}

