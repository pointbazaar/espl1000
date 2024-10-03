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

static vmcu_system_t* common(uint16_t addr, int16_t value, uint8_t redzone);

static vmcu_system_t* common(uint16_t addr, int16_t value, uint8_t redzone) {

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, addr));
	tacbuffer_append(b, makeTACStore(2, 1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer_with_redzone(b, addr, redzone);

	vmcu_system_step_n(system, 10);

	return system;
}

void test_compile_tac_store_case_8bit_value_8bit_addr_c7() {

	status_test_codegen("TAC_STORE (8 bit value, 8 bit addr = 0xc7)");

	const uint16_t addr = 0xc7;
	const uint8_t fixed_value = 0x2;
	const uint8_t redzone = 0xff;

	vmcu_system_t* system = common(addr, fixed_value, redzone);

	const uint8_t stored = vmcu_system_read_data(system, addr);

	assert_redzone(system, addr, 1, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_store_case_8bit_value_8bit_addr_c8() {

	status_test_codegen("TAC_STORE (8 bit value, 8 bit addr = 0xc8)");

	const uint16_t addr = 0xc8;
	const uint8_t fixed_value = 0x2;
	const uint8_t redzone = 0xff;

	vmcu_system_t* system = common(addr, fixed_value, redzone);

	const uint8_t stored = vmcu_system_read_data(system, addr);

	assert(stored == fixed_value);

	assert_redzone(system, addr, 1, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_store_case_16bit_value_8bit_addr_c7_f03() {

	status_test_codegen("TAC_STORE (16 bit value = 0xf03, 8 bit addr = 0xc7)");

	const uint8_t redzone = 0xff;
	const uint16_t addr = 0xc7;
	const uint16_t fixed_value = 0x0f03;

	vmcu_system_t* system = common(addr, fixed_value, redzone);

	const uint16_t stored = vmcu_system_read_data16(system, addr);

	assert(stored == fixed_value);

	assert_redzone(system, addr, 2, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_store_case_16bit_value_8bit_addr_c8_f03() {

	status_test_codegen("TAC_STORE (16 bit value = 0xf03, 8 bit addr = 0xc8)");

	const uint8_t redzone = 0xff;
	const uint16_t addr = 0xc7;
	const uint16_t fixed_value = 0x0f03;

	vmcu_system_t* system = common(addr, fixed_value, redzone);

	const uint16_t stored = vmcu_system_read_data16(system, addr);

	assert(stored == fixed_value);

	assert_redzone(system, addr, 2, redzone);

	vmcu_system_dtor(system);
}

void test_compile_tac_store_case_8bit_value_16bit_addr() {

	status_test_codegen("TAC_STORE (8 bit value, 16 bit addr)");

	const uint16_t addr = 1200;
	const uint8_t redzone = 0xff;

	for (int8_t fixed_value = 1; fixed_value < 3; fixed_value++) {

		vmcu_system_t* system = common(addr, fixed_value, redzone);

		const int8_t stored = vmcu_system_read_data(system, addr);

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_store_case_16bit_value_16bit_addr() {

	status_test_codegen("TAC_STORE (16 bit value, 16 bit addr)");

	const uint16_t addr = 1200;
	const uint8_t redzone = 0xff;

	for (uint16_t fixed_value = 0x0f00; fixed_value < 0x0f03; fixed_value++) {

		vmcu_system_t* system = common(addr, fixed_value, redzone);

		const uint16_t stored = vmcu_system_read_data16(system, addr);

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}
