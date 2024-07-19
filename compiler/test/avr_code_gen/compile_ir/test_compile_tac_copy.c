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

static void test_8bit();
static void test_16bit();

void test_compile_tac_copy() {

	test_8bit();
	test_16bit();
}

static void test_8bit() {

	status_test_codegen("TAC_COPY - 8 bit");

	for (int8_t fixed_value = 0; fixed_value < 20; fixed_value++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, fixed_value));
		tacbuffer_append(b, makeTACCopy(1, 0));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		assert(vmcu_system_read_gpr(system, 0) == fixed_value);

		vmcu_system_dtor(system);
	}
}

static void test_16bit() {

	status_test_codegen("TAC_COPY - 16 bit");

	for (uint16_t fixed_value = 0x100; fixed_value < 0x110; fixed_value++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, fixed_value));
		tacbuffer_append(b, makeTACCopy(1, 0));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);
		int8_t r1 = vmcu_system_read_gpr(system, 1);

		//printf("r0 0x%x %d,  r1 0x%x %d\n", r0, (uint8_t)r0, r1, (uint8_t)r1);

		assert((uint8_t)r0 == (fixed_value & 0xff));
		assert((uint8_t)r1 == (fixed_value & 0xff00) >> 8);

		vmcu_system_dtor(system);
	}
}
