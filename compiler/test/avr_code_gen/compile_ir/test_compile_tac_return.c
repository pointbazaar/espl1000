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

void test_compile_tac_return() {

	status_test_codegen("TAC_RETURN");

	for (int8_t value = 0x54; value < 0x58; value++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACSetupSP());
		tacbuffer_append(b, makeTACConst(0, value + 1));
		tacbuffer_append(b, makeTACConst(1, value));
		tacbuffer_append(b, makeTACConst(2, value - 1));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		//returned value should be in r0
		assert(vmcu_system_read_gpr(system, 0) == value);

		vmcu_system_dtor(system);
	}
}
