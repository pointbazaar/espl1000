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

static void case_minus();
static void case_not();
static void case_bitwise_neg();

void test_compile_tac_unary_op(){
	
	case_minus();
	case_not();
	case_bitwise_neg();
}

static void case_minus(){
	
	status_test_codegen("TAC_UNARY_OP -");
    
	const int8_t start = rand()%0xff;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_MINUS));
    tacbuffer_append(b, makeTACReturn(1));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
    
    vmcu_system_step_n(system, 10);
	
	const int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == -start);
	
	vmcu_system_dtor(system);
}

static void case_not(){
	
	status_test_codegen("TAC_UNARY_OP !");
    
	const int8_t start = 0xff; //start is true value
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_NOT));
    tacbuffer_append(b, makeTACReturn(1));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
    
    vmcu_system_step_n(system, 10);
	
	const int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == 0);
	
	vmcu_system_dtor(system);
}

static void case_bitwise_neg(){
	
	status_test_codegen("TAC_UNARY_OP ~");
    
	const int8_t start = rand()%0xff;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_BITWISE_NEG));
    tacbuffer_append(b, makeTACReturn(1));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);
    
    vmcu_system_step_n(system, 10);
	
	const int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == ~start);
	
	vmcu_system_dtor(system);
}
