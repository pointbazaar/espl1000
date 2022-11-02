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

void test_compile_tac_load_const_addr(){
	
	status_test_codegen("TAC_LOAD_CONST_ADDR");
	
	const uint16_t addr = 0x100+rand()%30;
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(1, 0x00));
    tacbuffer_append(b, makeTACLoadConstAddr(1, addr));
    tacbuffer_append(b, makeTACReturn(1));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
    
    //write value to be read later
    vmcu_system_write_data(system, addr, fixed_value);

    vmcu_system_step_n(system, 10);
        
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == fixed_value);
	
	vmcu_system_dtor(system);
}
