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

void test_compile_tac_store(){
	
	status_test_codegen("TAC_STORE");
	
	const uint16_t addr = 0xc7+rand()%10;
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(1, fixed_value));
    tacbuffer_append(b, makeTACConst(2, addr));
    tacbuffer_append(b, makeTACStore(2, 1));
    tacbuffer_append(b, makeTACReturn(1));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
    
    //write value to be read later
    vmcu_system_write_data(system, addr, fixed_value);

    vmcu_system_step_n(system, 10);
        
	int8_t r0 = vmcu_system_read_data(system, addr);
	
	assert(r0 == fixed_value);
	
	vmcu_system_dtor(system);
}
