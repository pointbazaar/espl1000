#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
//#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_compile_tac.h"

void test_compile_tac_return_case_8bit_0x54() {

	status_test_codegen("TAC_RETURN (8 bit value = 0x54)");

	const int8_t value = 0x54;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP());
	tacbuffer_append(b, makeTACConst(0, value + 1));
	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, value - 1));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	assert(vmcu_system_read_gpr(system, 0) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_return_case_8bit_0x55() {

	status_test_codegen("TAC_RETURN (8 bit value = 0x55)");

	const int8_t value = 0x55;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP());
	tacbuffer_append(b, makeTACConst(0, value + 1));
	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, value - 1));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	assert(vmcu_system_read_gpr(system, 0) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_return_case_16bit_0x5432() {

	status_test_codegen("TAC_RETURN (16 bit value = 0x5432)");

	const uint16_t value = 0x5432;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP());
	tacbuffer_append(b, makeTACConst(0, value + 1));
	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, value - 1));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 12);

	assert(vmcu_system_read_2_gpr(system, 0) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_return_case_16bit_0x4321() {

	status_test_codegen("TAC_RETURN (16 bit value = 0x4321)");

	const uint16_t value = 0x4321;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP());
	tacbuffer_append(b, makeTACConst(0, value + 1));
	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, value - 1));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 12);

	assert(vmcu_system_read_2_gpr(system, 0) == value);

	vmcu_system_dtor(system);
}
