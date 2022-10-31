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

static void case_true();
static void case_false();
static void case_mixed();

void test_compile_tac_if_goto(){
	
	case_true();
	case_false();
	case_mixed();
}

static void case_true(){
	
	status_test_codegen("TAC_IF_GOTO true");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const int8_t return_value = rand(); 
	
	//labels
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(1, 1));
    tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));
	
	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
	tacbuffer_append(b, makeTACGoto(lend));
	
	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACConst(0, return_value));
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    for(int i=0;i < 13; i++){
        vmcu_system_step(system);
	}
	
	//assert we have returned
	assert(vmcu_system_read_gpr(system, 0) == return_value);
	
	assert(vmcu_system_read_data(system, address1) == value);
	
	vmcu_system_dtor(system);
}

static void case_false(){
	
	status_test_codegen("TAC_IF_GOTO false");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const int8_t return_value = rand(); 
	
	//labels
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(1, 0));
    tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));
	
	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
	
	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACConst(0, return_value));
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    for(int i=0;i < 13; i++){
        vmcu_system_step(system);
	}
	
	//assert we have returned
	assert(vmcu_system_read_gpr(system, 0) == return_value);
	
	assert(vmcu_system_read_data(system, address1) != value);
	
	vmcu_system_dtor(system);
}

static void case_mixed(){
	
	status_test_codegen("TAC_IF_GOTO mixed");
	
	//we need to test 2 cases
	// - condition true, we branch
	// - condition false, we do not branch
	//we check by writing a fixed value to 2 known addresses in data space
	
	const int8_t value = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	const uint16_t address2 = address1 + 1;
	
	const int8_t return_value = rand(); 
	
	//labels
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACConst(0, value));
    
    tacbuffer_append(b, makeTACConst(1, 1));
    tacbuffer_append(b, makeTACIfGoto(1, l1)); //should branch
	tacbuffer_append(b, makeTACGoto(lend));
	
	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
    tacbuffer_append(b, makeTACConst(2, 0));
	tacbuffer_append(b, makeTACIfGoto(2, lend)); //should not branch
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address2, 0));
	tacbuffer_append(b, makeTACGoto(lend));
	
	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACConst(0, return_value));
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    for(int i=0;i < 20; i++){
        vmcu_system_step(system);
	}
	
	//assert we have returned
	assert(vmcu_system_read_gpr(system, 0) == return_value);
	
	//check that the values have arrived
	assert(vmcu_system_read_data(system, address1) == value);
	assert(vmcu_system_read_data(system, address2) == value);
	
	vmcu_system_dtor(system);
}
