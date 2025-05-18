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

void test_compile_tac_const_value_test_8bit() {

	status_test_codegen("TAC_CONST_VALUE - 8 bit");

	for (int8_t fixed_value = -40; fixed_value < 40; fixed_value += 10) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(1, fixed_value));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		//check that the value was written to any register
		bool found = false;

		for (int i = 0; i < 32; i++) {

			if (vmcu_system_read_gpr(system, i) == fixed_value) found = true;
		}

		assert(found);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_const_value_test_16bit() {

	status_test_codegen("TAC_CONST_VALUE - 16 bit");

	for (int16_t fixed_value = -10000; fixed_value < 10000; fixed_value += 2000) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(1, fixed_value));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		//check that the value was written to any register,
		//and the upper half to the reg above it
		bool found = false;

		for (int i = 0; i < 31; i++) {

			const int16_t value = (int16_t)vmcu_system_read_2_gpr(system, i);

			if (value == fixed_value) {
				found = true;
			}
		}

		assert(found);

		vmcu_system_dtor(system);
	}
}
