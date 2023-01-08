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

static void case_add();
static void case_sub();
static void case_and();
static void case_or();
static void case_xor();
static void case_shift_left();
static void case_shift_right();

void test_compile_tac_binary_op_immediate(){

	case_add();
	case_sub();
	case_and();
	case_or();
	case_xor();
	case_shift_left();
	case_shift_right();
}

static void case_add(){
	
	status_test_codegen("TAC_BINARY_OP_IMMEDIATE +");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start + change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_ADD, change));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void case_sub(){
	
	status_test_codegen("TAC_BINARY_OP_IMMEDIATE -");
	
	int8_t start = 40+rand()%10;
	int8_t change = 10+rand()%10;
	int8_t expected = start - change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SUB, change));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void case_and(){
	
	status_test_codegen("TAC_BINARY_OP_IMMEDIATE &");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start & change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_AND, change));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void case_or(){
	
	status_test_codegen("TAC_BINARY_OP_IMMEDIATE |");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start | change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_OR, change));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void case_xor(){

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE ^");

	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start ^ change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_XOR, change));
	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expected);

	vmcu_system_dtor(system);
}

static void case_shift_left(){

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE <<");

	uint8_t start = 1+rand()%0xff;

	for(int change = 1; change < 6; change++){

		uint8_t expected = start << change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_LEFT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8+change);

		uint8_t r0 = (uint8_t)vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

static void case_shift_right(){

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE >>");

	uint8_t start = 1+rand()%0xff;

	for(int change = 1; change < 6; change++){

		uint8_t expected = start >> change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_RIGHT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8+change);

		uint8_t r0 = (uint8_t)vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}
