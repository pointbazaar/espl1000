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

	tacbuffer_append(b, makeTACConst(0, 1, 0x1010));
	tacbuffer_append(b, makeTACConst(0, 2, addr));
	tacbuffer_append(b, makeTACLoad(0, 1, 2, (wide) ? 2 : 1));
	tacbuffer_append(b, makeTACReturn(0, 1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer_with_redzone(b, addr, redzone);

	vmcu_system_write_data(system, addr, fixed_value);
	if (wide) {
		vmcu_system_write_data(system, addr + 1, (fixed_value >> 8));
	}

	vmcu_system_step_n(system, 20);

	const uint16_t value = vmcu_system_read_2_gpr(system, 0);

	if (value != fixed_value) {
		printf("[0x%x] (may read 2 bytes) == 0x%x, expected 0x%x\n", addr, value, fixed_value);
	}

	assert(value == fixed_value);

	assert_redzone(system, addr, (wide) ? 2 : 1, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_load_case_8bit_addr_8bit_value() {

	status_test_codegen("TAC_LOAD (8 bit addr, 8 bit value)");

	const uint16_t addr = 0xc7;
	const uint8_t fixed_value = 0x33;

	common(addr, fixed_value, false);
}

void test_compile_tac_load_case_8bit_addr_16bit_value() {

	status_test_codegen("TAC_LOAD (8 bit addr, 16 bit value)");

	const uint16_t addr = 0xc7;
	const uint16_t fixed_value = 0x3456;

	common(addr, fixed_value, true);
}

void test_compile_tac_load_case_16bit_addr_8bit_value() {

	status_test_codegen("TAC_LOAD (16 bit addr, 8 bit value)");

	const uint16_t addr = 0x0103;
	const int8_t fixed_value = 0x20;

	common(addr, fixed_value, false);
}

void test_compile_tac_load_case_16bit_addr_16bit_value() {

	status_test_codegen("TAC_LOAD (16 bit addr, 16 bit value)");

	const uint16_t addr = 0x0103;
	const uint16_t fixed_value = 0x2087;

	common(addr, fixed_value, true);
}
