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

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tvalue = makeTACConst(1, fixed_value); 
    struct TAC* taddr = makeTACConst(2, addr);
    struct TAC* tac1 = makeTACStore(2, 1);
    struct TAC* tac2 = makeTACReturn(1);
    
    tacbuffer_append(buffer, tvalue);
    tacbuffer_append(buffer, taddr);
    tacbuffer_append(buffer, tac1);
    tacbuffer_append(buffer, tac2);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
    
    //write value to be read later
    vmcu_system_write_data(system, addr, fixed_value);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
        
	int8_t r0 = vmcu_system_read_data(system, addr);
	
	assert(r0 == fixed_value);
	
	vmcu_system_dtor(system);
}
