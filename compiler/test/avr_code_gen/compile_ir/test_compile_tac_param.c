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

void test_compile_tac_param_case_8bit() {

	status_test_codegen("TAC_PARAM - (8 bit)");

	//test that the value gets pushed
	//and the stack pointer decrements

	const int8_t fixed_value = 0x3c;

	const uint8_t SPL_ADDR = 0x5d;
	const uint8_t SPH_ADDR = 0x5e;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP(0));
	tacbuffer_append(b, makeTACConst(0, 0, fixed_value));
	tacbuffer_append(b, makeTACParam(0, 0, false, 0, false));
	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 4);

	int sp_old = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;

	vmcu_system_step_n(system, 3);

	int sp = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;

	//assert that SP was decremented
	assert(sp == sp_old - 1);

	//assert that fixed_value is on the stack
	assert(vmcu_system_read_data(system, sp_old) == fixed_value);

	vmcu_system_dtor(system);
}

void test_compile_tac_param_case_16bit() {

	status_test_codegen("TAC_PARAM - (16 bit)");

	//test that the value gets pushed
	//and the stack pointer decrements

	const uint16_t fixed_value = 0x0abc;

	const uint8_t SPL_ADDR = 0x5d;
	const uint8_t SPH_ADDR = 0x5e;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupSP(0));
	tacbuffer_append(b, makeTACConst(0, 0, fixed_value));
	tacbuffer_append(b, makeTACParam(0, 0, true, 0, false));
	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 4);

	const int sp_old = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;

	vmcu_system_step_n(system, 4);

	const int sp = vmcu_system_read_data(system, SPL_ADDR) | vmcu_system_read_data(system, SPH_ADDR) << 8;

	//assert that SP was decremented
	assert(sp == sp_old - 2);

	//assert that fixed_value is on the stack
	assert((uint8_t)vmcu_system_read_data(system, sp_old) == 0x0a);
	assert((uint8_t)vmcu_system_read_data(system, sp_old - 1) == 0xbc);

	vmcu_system_dtor(system);
}
