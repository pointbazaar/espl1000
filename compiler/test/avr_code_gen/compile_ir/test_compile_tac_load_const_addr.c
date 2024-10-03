#include <stdio.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"

#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_compile_tac.h"

static void common(const uint16_t addr, const uint16_t fixed_value, bool wide) {

	const uint8_t redzone = 0xff;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0xf1f1));
	tacbuffer_append(b, makeTACLoadConstAddr(1, addr));
	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer_with_redzone(b, addr, redzone);

	//write value to be read later
	vmcu_system_write_data(system, addr, fixed_value & 0xff);
	if (wide) {
		vmcu_system_write_data(system, addr + 1, (fixed_value >> 8) & 0xff);
	}

	vmcu_system_step_n(system, 10);

	uint16_t value;
	if (wide) {
		value = vmcu_system_read_2_gpr(system, 0);
	} else {
		value = vmcu_system_read_gpr(system, 0);
	}

	if (value != fixed_value) {
		printf("[0x%x] (may read 2 bytes) = 0x%x, expected 0x%x\n", addr, value, fixed_value);
	}

	assert(value == fixed_value);

	assert_redzone(system, addr, (wide) ? 2 : 1, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_load_const_addr_case_8bit_value_0x55() {

	status_test_codegen("TAC_LOAD_CONST_ADDR (8 bit value = 0x55)");

	const uint16_t addr = 0x100 + 17;
	const uint8_t fixed_value = 0x55;

	common(addr, fixed_value, false);
}

void test_compile_tac_load_const_addr_case_8bit_value_0x56() {

	status_test_codegen("TAC_LOAD_CONST_ADDR (8 bit value = 0x56)");

	const uint16_t addr = 0x100 + 17;
	const uint8_t fixed_value = 0x56;

	common(addr, fixed_value, false);
}

void test_compile_tac_load_const_addr_case_16bit_value_0x1234() {

	status_test_codegen("TAC_LOAD_CONST_ADDR (16 bit value = 0x1234)");

	const uint16_t addr = 0x100 + 17;
	const uint16_t fixed_value = 0x1234;

	common(addr, fixed_value, true);
}

void test_compile_tac_load_const_addr_case_16bit_value_0x4321() {

	status_test_codegen("TAC_LOAD_CONST_ADDR (16 bit value = 0x4321)");

	const uint16_t addr = 0x100 + 17;
	const uint16_t fixed_value = 0x4321;

	common(addr, fixed_value, true);
}
