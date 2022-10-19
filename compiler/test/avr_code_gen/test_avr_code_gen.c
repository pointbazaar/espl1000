#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "test_avr_code_gen.h"
#include "test_avr_code_gen_util.h"

//unit tests forward declarations
static void test_stack_pointer_setup_correctly();
static void test_reaches_endloop();

static void test_tac_nop();
static void test_tac_const_value();
static void test_tac_store_const_addr();
static void test_tac_load_const_addr();

static void status_test_codegen(char* msg){
    printf(" - [TEST] avr codegen %s\n", msg);
}

void test_suite_avr_code_gen(){

    test_stack_pointer_setup_correctly();
    test_reaches_endloop();
    
    test_tac_nop();
    test_tac_const_value();
    test_tac_store_const_addr();
    test_tac_load_const_addr();
    //TODO: add more tests to cover all TAC
}

// --- UNIT TESTS ---

static void test_stack_pointer_setup_correctly(){

    status_test_codegen("stackpointer setup correctly");

    struct TACBuffer* buffer = tacbuffer_ctor();

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    //step 4 times
    for(int i=0;i < 4;i++)
        vmcu_system_step(system);

    //assert SPH:SPL == 0x085f

    //the 'out' instruction receives an offset to the base address off the io space within the data space
    const uint32_t io_offset = 0x20;
    int8_t SPH_value = vmcu_system_read_data(system, io_offset+0x3e);
    int8_t SPL_value = vmcu_system_read_data(system, io_offset+0x3d);

    //printf("SPH = %04x, SPL = %04x\n", SPH_value, SPL_value);

    assert((SPH_value << 8 | SPL_value) == 0x085f);

    vmcu_system_dtor(system);
}

static void test_reaches_endloop(){

    status_test_codegen("reaches endloop");

    //create a TACBuffer* where we put the TAC that in the end will be compiled

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tconst = makeTACConst(1, 0);
    struct TAC* treturn = makeTACReturn("t1");

	tacbuffer_append(buffer, tconst, false);
	tacbuffer_append(buffer, treturn, false);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    //step through the system and see that it should converge on a value for pc (endloop)

    for(int i=0;i < 30; i++)
        vmcu_system_step(system);

    int32_t pc1 = (int32_t)vmcu_system_get_pc(system);
    vmcu_system_step(system);
    int32_t pc2 = (int32_t)vmcu_system_get_pc(system);

    //check it
    assert(pc1 == pc2);

    //check that pc remains in range
    for(int i=0;i < 10; i++) {

        vmcu_system_step(system);
        int32_t pc = (int32_t)vmcu_system_get_pc(system);

        assert(pc == pc1);
    }

    vmcu_system_dtor(system);
}

static void test_tac_nop(){
	
	status_test_codegen("TAC_NOP");
	
	//test that register etc. do not change with TAC_NOP
	//this test is also intended for completeness

    struct TACBuffer* buffer = tacbuffer_ctor();
    int n = 8;

    struct TAC* tac0 = makeTACConst(1, 0x00);
    struct TAC* tac2 = makeTACReturn("t1");

    tacbuffer_append(buffer, tac0, false);
    
    for(int i=0; i < n; i++){
		tacbuffer_append(buffer, makeTACNop(), false);
	}
    
    tacbuffer_append(buffer, tac2, false);

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

static void test_tac_const_value(){
	
	status_test_codegen("TAC_CONST_VALUE");
	
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tac0 = makeTACConst(1, fixed_value);
    struct TAC* tac1 = makeTACReturn("t1");
    
    tacbuffer_append(buffer, tac0, false);
    tacbuffer_append(buffer, tac1, false);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
        
	//check that the value was written to any register
	bool found = false;
	
	for(int i = 0; i < 32; i++){
		
		if(vmcu_system_read_gpr(system, i) == fixed_value) found = true;
	}
	
	assert(found);
	
	vmcu_system_dtor(system);
}

static void test_tac_store_const_addr(){
	
	status_test_codegen("TAC_STORE_CONST_ADDR");
	
	const uint8_t addr = 0x40+rand()%0xf;
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tac0 = makeTACConst(1, fixed_value);
    struct TAC* tac1 = makeTACStoreConstAddr(addr, "t1");
    struct TAC* tac2 = makeTACReturn("t1");
	
	tacbuffer_append(buffer, tac0, false);
    tacbuffer_append(buffer, tac1, false);
    tacbuffer_append(buffer, tac2, false);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
        
	//check that the value was written

	int8_t value = vmcu_system_read_data(system, addr);
	
	assert(value == fixed_value);
	
	vmcu_system_dtor(system);
}

static void test_tac_load_const_addr(){
	
	status_test_codegen("TAC_LOAD_CONST_ADDR");
	
	const uint8_t addr = 0x40+rand()%0xf;
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tac0 = makeTACConst(1, 0x00); //value here not important
    struct TAC* tac1 = makeTACLoadConstAddr("t1", addr);
    struct TAC* tac2 = makeTACReturn("t1");
    
    tacbuffer_append(buffer, tac0, false);
    tacbuffer_append(buffer, tac1, false);
    tacbuffer_append(buffer, tac2, false);

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
    
    //write value to be read later
    vmcu_system_write_data(system, addr, fixed_value);

    for(int i=0;i < 10; i++){
        vmcu_system_step(system);
	}
        
	//check that the value was read to any register
	bool found = false;
	
	for(int i = 0; i < 32; i++){
		
		if(vmcu_system_read_gpr(system, i) == fixed_value) found = true;
	}
	
	assert(found);
	
	vmcu_system_dtor(system);
}
