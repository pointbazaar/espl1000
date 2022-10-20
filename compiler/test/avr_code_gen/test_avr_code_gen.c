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
static void test_tac_binary_op_immediate();
static void test_tac_unary_op();
static void test_tac_binary_op();
static void test_tac_goto();
static void test_tac_if_goto();
static void test_tac_return();

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
    test_tac_binary_op_immediate();
    test_tac_unary_op();
    test_tac_binary_op();
    test_tac_goto();
    test_tac_if_goto();
    test_tac_return();
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

	tacbuffer_append(buffer, makeTACConst(1, 0));
	tacbuffer_append(buffer, makeTACReturn("t1"));

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

static void test_tac_const_value(){
	
	status_test_codegen("TAC_CONST_VALUE");
	
	const int8_t fixed_value = rand()%0xff;

    struct TACBuffer* buffer = tacbuffer_ctor();

    struct TAC* tac0 = makeTACConst(1, fixed_value);
    struct TAC* tac1 = makeTACReturn("t1");
    
    tacbuffer_append(buffer, tac0);
    tacbuffer_append(buffer, tac1);

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
	
	tacbuffer_append(buffer, tac0);
    tacbuffer_append(buffer, tac1);
    tacbuffer_append(buffer, tac2);

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
    
    tacbuffer_append(buffer, tac0);
    tacbuffer_append(buffer, tac1);
    tacbuffer_append(buffer, tac2);

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

static void test_tac_binary_op_immediate(){

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE");
	
	//we test +,-,|,& operators
	uint8_t change = 40;
	
	int8_t starts[] = {rand()%100,rand()%100,rand()%100,rand()%100};
	int8_t changes[] = {rand()%change,rand()%change,rand()%change,rand()%change};
	int8_t expected[] = {
		starts[0] + changes[0],
		starts[1] - changes[1],
		starts[2] | changes[2],
		starts[3] & changes[3],
	};
	
	char* temps[] = {"t0","t1","t2","t3"};
	enum TAC_OP ops[] = {TAC_OP_ADD, TAC_OP_SUB, TAC_OP_OR, TAC_OP_AND};

    struct TACBuffer* buffer = tacbuffer_ctor();
    
    //add all the operations
    for(int i=0; i < 4; i++){
		
		tacbuffer_append(buffer, makeTACConst(i, starts[i]));
		tacbuffer_append(buffer, makeTACBinOpImmediate(temps[i], ops[i], changes[i]));
	}
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

	//keep track of which values have initialized and which changed
	bool has_init[]    = {false,false,false,false};
	bool has_changed[] = {false,false,false,false};

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
        
        for(int k = 0; k < 32; k++){
			
			const int8_t value = vmcu_system_read_gpr(system, k);
			
			for(int j = 0; j < 4; j++){
				
				if(has_init[j] && value == expected[j]) 
					has_changed[j] = true;
			
				if(value == starts[j]) 
					has_init[j] = true;
			}
		}
	}
	
	for(int j = 1; j < 2; j++){
		
		assert(has_init[j]);
		assert(has_changed[j]);
	}
	
	vmcu_system_dtor(system);
}

static void test_tac_unary_op(){
	
	status_test_codegen("TAC_UNARY_OP");
    
	const int8_t start = rand()%0xff;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACConst(0, start));
	
	tacbuffer_append(buffer, makeTACUnaryOp("t1","t0",TAC_OP_UNARY_MINUS));
    tacbuffer_append(buffer, makeTACUnaryOp("t2","t0",TAC_OP_UNARY_NOT));
    tacbuffer_append(buffer, makeTACUnaryOp("t3","t0",TAC_OP_UNARY_BITWISE_NEG));
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
    
    bool has1 = false;
	bool has2 = false;
	bool has3 = false;

    for(int i=0;i < 16; i++){
		
        vmcu_system_step(system);
        
        for(int k = 0; k < 32; k++){
			
			const int8_t value = vmcu_system_read_gpr(system, k);
			
			if(value == -start) has1 = true;
			if(value == !start) has2 = true;
			if(value == ~start) has3 = true;
		}
	}
	
	assert(has1);
	assert(has2);
	assert(has3);
	
	vmcu_system_dtor(system);
}

static void test_tac_binary_op(){
	
	status_test_codegen("TAC_BINARY_OP");
	
	//TODO: test * (multiply)
	//we test +,-,|,& operators
	uint8_t change = 40;
	
	int8_t starts[] = {rand()%100,rand()%100,rand()%100,rand()%100};
	int8_t changes[] = {rand()%change,rand()%change,rand()%change,rand()%change};
	int8_t expected[] = {
		starts[0] + changes[0],
		starts[1] - changes[1],
		starts[2] | changes[2],
		starts[3] & changes[3],
	};
	
	char* temps[] = {"t0","t1","t2","t3","t4","t5","t6","t7"};
	enum TAC_OP ops[] = {TAC_OP_ADD, TAC_OP_SUB, TAC_OP_OR, TAC_OP_AND};

    struct TACBuffer* buffer = tacbuffer_ctor();
    
    //add all the operations
    for(int i=0; i < 4; i++){
		
		tacbuffer_append(buffer, makeTACConst(i, starts[i]));
		tacbuffer_append(buffer, makeTACConst(i+4, changes[i]));
		tacbuffer_append(buffer, makeTACBinOp(temps[i], ops[i], temps[i+4]));
	}
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

	//keep track of which values have initialized and which changed
	bool has_init[]    = {false,false,false,false};
	bool has_changed[] = {false,false,false,false};

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
        
        for(int k = 0; k < 32; k++){
			
			const int8_t value = vmcu_system_read_gpr(system, k);
			
			for(int j = 0; j < 4; j++){
				
				if(has_init[j] && value == expected[j]) 
					has_changed[j] = true;
			
				if(value == starts[j]) 
					has_init[j] = true;
			}
		}
	}
	
	for(int j = 1; j < 2; j++){
		
		assert(has_init[j]);
		assert(has_changed[j]);
	}
	
	vmcu_system_dtor(system);
}

static void test_tac_goto(){
	
	status_test_codegen("TAC_GOTO");
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACLabel(43));
	tacbuffer_append(buffer, makeTACGoto(43));
    tacbuffer_append(buffer, makeTACReturn("t0"));

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

static void test_tac_if_goto(){
	
	status_test_codegen("TAC_IF_GOTO");
	
	//we need to test 2 cases
	// - condition true, we branch
	// - condition false, we do not branch
	//we check by writing a fixed value to 2 known addresses in data space
	
	const uint8_t value = rand()%0xff;
	const uint16_t address1 = 0x46;
	const uint16_t address2 = 0x47;
	
	//labels
	const uint16_t l1 = rand()%0xff;
	const uint16_t lend = l1+2;
	
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
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, value));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 20; i++){
        vmcu_system_step(system);
	}
	
	//check that the values have arrived
	assert(vmcu_system_read_data(system, address1) == value);
	assert(vmcu_system_read_data(system, address2) == value);
	
	vmcu_system_dtor(system);
}

static void test_tac_return(){

	status_test_codegen("TAC_RETURN");
	
	const int8_t value = rand()%0xff;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(0, value));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 20; i++){
        vmcu_system_step(system);
	}
	
	//returned value should be in r0
	assert(vmcu_system_read_gpr(system, 0) == value);
	
	vmcu_system_dtor(system);
}
