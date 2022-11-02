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

static void test_compile_tac_binary_op_neq_true();
static void test_compile_tac_binary_op_neq_false();
static void test_compile_tac_binary_op_lt_true();
static void test_compile_tac_binary_op_lt_false();
static void test_compile_tac_binary_op_eq_true();
static void test_compile_tac_binary_op_eq_false();

void test_compile_tac_binary_op(){
	
	test_compile_tac_binary_op_add();
	test_compile_tac_binary_op_sub();
	test_compile_tac_binary_op_and();
	test_compile_tac_binary_op_or();
	test_compile_tac_binary_op_xor();
	
	test_compile_tac_binary_op_neq_true();
	test_compile_tac_binary_op_neq_false();
	test_compile_tac_binary_op_lt_true();
	test_compile_tac_binary_op_lt_false();
	test_compile_tac_binary_op_eq_true();
	test_compile_tac_binary_op_eq_false();
}

static void test_compile_tac_binary_op_add(){
	
	status_test_codegen("TAC_BINARY_OP +");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start + change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_ADD, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system,8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_sub(){
	
	status_test_codegen("TAC_BINARY_OP -");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start - change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_SUB, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_and(){
	
	status_test_codegen("TAC_BINARY_OP &");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start & change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_AND, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_or(){
	
	status_test_codegen("TAC_BINARY_OP |");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start | change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_OR, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_xor(){
	
	status_test_codegen("TAC_BINARY_OP ^");
	
	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start ^ change;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_XOR, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == expected);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_true(){
	
	status_test_codegen("TAC_BINARY_OP != true");
	
	int8_t value1 = rand()%0xf;
	int8_t value2 = value1+1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 != 0);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_false(){

	status_test_codegen("TAC_BINARY_OP != false");
	
	int8_t value1 = rand()%0xf;
	int8_t value2 = value1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 8);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == 0);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_true(){
	
	status_test_codegen("TAC_BINARY_OP < true");
	
	int8_t value1 = rand()%0xf;
	int8_t value2 = value1+1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 != 0);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_false(){
	
	status_test_codegen("TAC_BINARY_OP < false");
	
	int8_t value1 = rand()%0xf;
	int8_t value2 = value1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == 0);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_true(){
	
	status_test_codegen("TAC_BINARY_OP == true");
	
	int8_t value1 = rand()%0xff;
	int8_t value2 = value1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 != 0);
	
	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_false(){
	
	status_test_codegen("TAC_BINARY_OP == false");
	
	int8_t value1 = rand()%0xff;
	int8_t value2 = value1+1;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    vmcu_system_step_n(system, 10);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == 0);
	
	vmcu_system_dtor(system);
}
