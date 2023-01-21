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

static void test_8bit_value_8bit_addr();
static void test_16bit_value_8bit_addr();
static void test_8bit_value_16bit_addr();
static void test_16bit_value_16bit_addr();

static vmcu_system_t* common(uint16_t addr, int16_t value);

void test_compile_tac_store(){

	test_8bit_value_8bit_addr();
	test_16bit_value_8bit_addr();
	test_8bit_value_16bit_addr();
	test_16bit_value_16bit_addr();
}

static vmcu_system_t* common(uint16_t addr, int16_t value){

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, value));
	tacbuffer_append(b, makeTACConst(2, addr));
	tacbuffer_append(b, makeTACStore(2, 1));

	tacbuffer_append(b, makeTACReturn(1));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	return system;
}

static void test_8bit_value_8bit_addr(){

	status_test_codegen("TAC_STORE (8 bit value, 8 bit addr)");

	const uint16_t addr = 0xc7+rand()%10;

	for(int8_t fixed_value = 1; fixed_value < 10; fixed_value++){

		vmcu_system_t* system = common(addr, fixed_value);

		const int8_t stored = vmcu_system_read_data(system, addr);

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}

static void test_16bit_value_8bit_addr(){

	status_test_codegen("TAC_STORE (16 bit value, 8 bit addr)");

	const uint16_t addr = 0xc7+rand()%10;

	for(uint16_t fixed_value = 0x0f00; fixed_value < 0x0f0f; fixed_value++){

		vmcu_system_t* system = common(addr, fixed_value);

		const uint8_t low = vmcu_system_read_data(system, addr);
		const uint8_t high = vmcu_system_read_data(system, addr+1);

		const int16_t stored = (high << 8) | low;

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}

static void test_8bit_value_16bit_addr(){

	status_test_codegen("TAC_STORE (8 bit value, 16 bit addr)");

	const uint16_t addr = 1200+rand()%10;

	for(int8_t fixed_value = 1; fixed_value < 10; fixed_value++){

		vmcu_system_t* system = common(addr, fixed_value);

		const int8_t stored = vmcu_system_read_data(system, addr);

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}

static void test_16bit_value_16bit_addr(){

	status_test_codegen("TAC_STORE (16 bit value, 16 bit addr)");

	const uint16_t addr = 1200+rand()%10;

	for(uint16_t fixed_value = 0x0f00; fixed_value < 0x0f0f; fixed_value++){

		vmcu_system_t* system = common(addr, fixed_value);

		const uint8_t low = vmcu_system_read_data(system, addr);
		const uint8_t high = vmcu_system_read_data(system, addr+1);

		const int16_t stored = (high << 8) | low;

		assert(stored == fixed_value);

		vmcu_system_dtor(system);
	}
}
