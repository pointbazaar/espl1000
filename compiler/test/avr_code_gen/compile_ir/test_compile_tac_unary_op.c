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

static void case_minus_8bit();
static void case_minus_16bit();

static void case_not();

static void case_bitwise_neg_8bit();
static void case_bitwise_neg_16bit();

void test_compile_tac_unary_op(){

	case_minus_8bit();
	case_minus_16bit();

	case_not();

	case_bitwise_neg_8bit();
	case_bitwise_neg_16bit();
}

static void case_minus_8bit(){

	status_test_codegen("TAC_UNARY_OP - (8 bit)");

	for(int8_t start = -40; start < 40; start += 10){

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
}

static void case_minus_16bit(){

	status_test_codegen("TAC_UNARY_OP - (16 bit)");

	for(int16_t start = -3000; start < 3000; start += 1000){

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_MINUS));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		const uint8_t r0 = vmcu_system_read_gpr(system, 0);
		const uint8_t r1 = vmcu_system_read_gpr(system, 1);

		int16_t actual = (r1 << 8) | r0;

		assert(actual == -start);

		vmcu_system_dtor(system);
	}
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

static void case_bitwise_neg_8bit(){

	status_test_codegen("TAC_UNARY_OP ~ (8 bit)");

	const uint8_t start = rand()%0xff;
	const uint8_t expect = ~start;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_BITWISE_NEG));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	const uint8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expect);

	vmcu_system_dtor(system);
}

static void case_bitwise_neg_16bit(){

	status_test_codegen("TAC_UNARY_OP ~ (16 bit)");

	const uint16_t start = (0x3b << 8) | rand()%0xff;
	const uint16_t expect = ~start;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1,0,TAC_OP_UNARY_BITWISE_NEG));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	const uint8_t r0 = vmcu_system_read_gpr(system, 0);
	const uint8_t r1 = vmcu_system_read_gpr(system, 1);

	const uint16_t actual = (r1 << 8) | r0;

	assert(actual == expect);

	vmcu_system_dtor(system);
}
