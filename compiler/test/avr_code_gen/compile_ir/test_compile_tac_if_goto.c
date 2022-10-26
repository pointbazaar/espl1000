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

void test_compile_tac_if_goto(){
	
	status_test_codegen("TAC_IF_GOTO");
	
	//we need to test 2 cases
	// - condition true, we branch
	// - condition false, we do not branch
	//we check by writing a fixed value to 2 known addresses in data space
	
	const int8_t value = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	const uint16_t address2 = address1 + 1;
	
	//labels
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(0, value));
    
    tacbuffer_append(buffer, makeTACConst(1, 1));
    tacbuffer_append(buffer, makeTACIfGoto("t1", l1)); //should branch
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
    tacbuffer_append(buffer, makeTACConst(2, 0));
	tacbuffer_append(buffer, makeTACIfGoto("t2", lend)); //should not branch
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address2, "t0"));
	tacbuffer_append(buffer, makeTACGoto(lend));//DEBUG
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, value));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	//check that the values have arrived
	assert(vmcu_system_read_data(system, address1) == value);
	assert(vmcu_system_read_data(system, address2) == value);
	
	vmcu_system_dtor(system);
}
