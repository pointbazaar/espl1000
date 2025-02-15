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

void test_compile_tac_call_case_recurses() {

	status_test_codegen("TAC_CALL - recurse");

	const int8_t value = 0x34;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACLabelFunction(0));
	tacbuffer_append(b, makeTACConst(0, value));

	//tacbuffer_append(b, makeTACCall(1, "main"));
	tacbuffer_append(b, makeTACCall(1, 0));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	for (int i = 0; i < 40; i++) {
		vmcu_system_step(system);

		//r0 should never have that value, as we are never reaching
		//the return, always recursing
		assert(vmcu_system_read_gpr(system, 0) != value);
	}

	vmcu_system_dtor(system);
}

void test_compile_tac_call_case_returns_value_8bit() {

	status_test_codegen("TAC_CALL - returns value (8 bit)");

	const int8_t value = 0x63;

	struct TACBuffer* b = tacbuffer_ctor();

	//main
	tacbuffer_append(b, makeTACLabelFunction(0));
	tacbuffer_append(b, makeTACConst(0, value));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	assert(vmcu_system_read_gpr(system, 0) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_call_case_returns_value_16bit() {

	status_test_codegen("TAC_CALL - returns value (16 bit)");

	const uint16_t value = 0x6385;

	struct TACBuffer* b = tacbuffer_ctor();

	//main
	tacbuffer_append(b, makeTACLabelFunction(0));
	tacbuffer_append(b, makeTACConst(0, value));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	const uint16_t retval = vmcu_system_read_2_gpr(system, 0);

	if (retval != value) {
		printf("retval = 0x%x, value = 0x%x\n)", retval, value);
	}

	assert(retval == value);

	vmcu_system_dtor(system);
}
