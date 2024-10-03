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

void test_compile_tac_copy_case_8bit() {

	status_test_codegen("TAC_COPY - 8 bit");

	for (int8_t fixed_value = 0; fixed_value < 5; fixed_value++) {

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

void test_compile_tac_copy_case_16bit() {

	status_test_codegen("TAC_COPY - 16 bit");

	for (uint16_t fixed_value = 0x100; fixed_value < 0x105; fixed_value++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, fixed_value));
		tacbuffer_append(b, makeTACCopy(1, 0));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		const uint16_t value = vmcu_system_read_2_gpr(system, 0);

		assert(value == fixed_value);

		vmcu_system_dtor(system);
	}
}
