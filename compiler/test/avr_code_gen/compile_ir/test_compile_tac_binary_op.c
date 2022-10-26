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

static void test_compile_tac_binary_op_add();
static void test_compile_tac_binary_op_sub();
static void test_compile_tac_binary_op_and();
static void test_compile_tac_binary_op_or();
static void test_compile_tac_binary_op_xor();

void test_compile_tac_binary_op(){
	
	test_compile_tac_binary_op_add();
	test_compile_tac_binary_op_sub();
	test_compile_tac_binary_op_and();
	test_compile_tac_binary_op_or();
	test_compile_tac_binary_op_xor();
}

static void test_compile_tac_binary_op_add(){
	
	status_test_codegen("TAC_BINARY_OP +");
	
	int8_t start = rand()%100;
	int8_t change = rand()%40;
	int8_t expected = start + change;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACConst(0, start));
	tacbuffer_append(buffer, makeTACConst(1, change));
	tacbuffer_append(buffer, makeTACBinOp("t0", TAC_OP_ADD, "t1"));
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_sub(){
	
	status_test_codegen("TAC_BINARY_OP -");
	
	int8_t start = rand()%100;
	int8_t change = rand()%40;
	int8_t expected = start - change;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACConst(0, start));
	tacbuffer_append(buffer, makeTACConst(1, change));
	tacbuffer_append(buffer, makeTACBinOp("t0", TAC_OP_SUB, "t1"));
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_and(){
	
	status_test_codegen("TAC_BINARY_OP &");
	
	int8_t start = rand()%100;
	int8_t change = rand()%40;
	int8_t expected = start & change;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACConst(0, start));
	tacbuffer_append(buffer, makeTACConst(1, change));
	tacbuffer_append(buffer, makeTACBinOp("t0", TAC_OP_AND, "t1"));
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_or(){
	//TODO
}

static void test_compile_tac_binary_op_xor(){
	//TODO
}
