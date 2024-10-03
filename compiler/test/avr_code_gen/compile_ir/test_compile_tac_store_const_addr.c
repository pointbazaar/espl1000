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

static void common(const uint16_t addr, const uint16_t fixed_value, bool wide) {

	//const uint16_t addr = 0x118;
	//const uint8_t fixed_value = 0x44;
	const uint8_t redzone = 0xff;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, fixed_value));
	tacbuffer_append(b, makeTACStoreConstAddr(addr, 1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer_with_redzone(b, addr, redzone);

	vmcu_system_step_n(system, 10);

	uint16_t value;
	if (wide) {
		value = vmcu_system_read_data16(system, addr);
	} else {
		value = vmcu_system_read_data(system, addr);
	}

	if (value != fixed_value) {
		printf("[0x%x] == 0x%x, expected 0x%x\n", addr, value, fixed_value);
	}

	//check that the value was written
	assert(value == fixed_value);

	assert_redzone(system, addr, (wide) ? 2 : 1, redzone);

	vmcu_system_dtor(system);
}
void test_compile_tac_store_const_addr_case_8bit_value_addr_0x118() {

	status_test_codegen("TAC_STORE_CONST_ADDR (8 bit value, address = 0x118)");

	const uint16_t addr = 0x118;
	const uint8_t fixed_value = 0x44;
	common(addr, fixed_value, false);
}

void test_compile_tac_store_const_addr_case_8bit_value_addr_0x119() {

	status_test_codegen("TAC_STORE_CONST_ADDR (8 bit value, address = 0x119)");

	const uint16_t addr = 0x119;
	const uint8_t fixed_value = 0x45;
	common(addr, fixed_value, false);
}
void test_compile_tac_store_const_addr_case_16bit_value_addr_0x118() {

	status_test_codegen("TAC_STORE_CONST_ADDR (16 bit value, address = 0x118)");

	const uint16_t addr = 0x118;
	const uint16_t fixed_value = 0x4567;
	common(addr, fixed_value, true);
}

void test_compile_tac_store_const_addr_case_16bit_value_addr_0x119() {

	status_test_codegen("TAC_STORE_CONST_ADDR (16 bit value, address = 0x119)");

	const uint16_t addr = 0x119;
	const uint16_t fixed_value = 0x5678;
	common(addr, fixed_value, true);
}
