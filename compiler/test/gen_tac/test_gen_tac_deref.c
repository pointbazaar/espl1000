#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void test_gen_tac_deref_read_8bit_case_const_addr_single(uint16_t address, uint8_t value) {

	char snippet[200];
	sprintf(snippet, "fn main() -> int { *uint8 x = %d; uint8 y = *x; return 0; }", address);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_write_data(system, address, value);

	vmcu_system_step_n(system, 40);

	//assert that the load has happened
	//and that the value is in a register or in the stack frame

	bool found = false;

	for (int i = 0; i < 32; i++) {
		uint8_t value1 = vmcu_system_read_gpr(system, i);
		if (value1 == value) found = true;
	}

	assert(found);

	vmcu_system_dtor(system);
}

void test_gen_tac_deref_read_8bit_case_const_addr() {

	status_test_codegen_tac("Deref - 8 bit read - const Address");

	test_gen_tac_deref_read_8bit_case_const_addr_single(0x100, 0x34);
	test_gen_tac_deref_read_8bit_case_const_addr_single(0x101, 0x83);
	test_gen_tac_deref_read_8bit_case_const_addr_single(0x102, 0x29);
	test_gen_tac_deref_read_8bit_case_const_addr_single(0x103, 0x58);
}

void test_gen_tac_deref_read_16bit_case_const_addr() {

	status_test_codegen_tac("Deref - 16 bit read - const Address");
	const uint16_t address = 0x100;

	char snippet[200];
	sprintf(snippet, "fn main() -> int { *uint16 x = %d; uint16 y = *x; return 0; }", address);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_write_data16(system, address, 0x3923);

	vmcu_system_step_n(system, 40);

	//assert that the load has happened
	//and that the value is in a register or in the stack frame

	bool found = false;

	for (int i = 0; i < 32; i++) {
		if (vmcu_system_read_gpr(system, i) == 0x23 && vmcu_system_read_gpr(system, i + 1) == 0x39) {
			found = true;
		}
	}

	assert(found);

	vmcu_system_dtor(system);
}

void test_gen_tac_deref_case_variable_addr() {

	status_test_codegen_tac("Deref - 8 bit read - variable Address");

	//we need an address here which is small enough
	//to fit into a register. 0xc7 is usable.
	for (uint16_t address = 0xc7; address < 0xcb; address++) {

		const int8_t value = 0x38;

		char snippet[200];
		sprintf(snippet, "fn main() -> int { *int8 y = 3+%d; int8 x = *y; return x; }", address);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//prepare a value in the location
		vmcu_system_write_data(system, address + 3, value);

		//vmcu_system_step_n(system, 30);

		for (int i = 0; i < 40; i++) {

			vmcu_system_step(system);
		}

		//return value in r0
		int8_t actual = vmcu_system_read_gpr(system, 0);

		assert(value == actual);

		vmcu_system_dtor(system);
	}
}

static void test_gen_tac_deref_write_case_const_addr_single(uint16_t address, uint8_t value) {

	char snippet[200];
	sprintf(snippet, "fn main() -> int { *uint8 x = %d; *x = %d; return 0; }", address, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 40);

	const uint8_t actual = vmcu_system_read_data(system, address);

	assert(actual == value);

	vmcu_system_dtor(system);
}

void test_gen_tac_deref_write_8bit_case_const_addr() {

	status_test_codegen_tac("Deref - 8 bit write - const Address");

	test_gen_tac_deref_write_case_const_addr_single(0x100, 0x45);
	test_gen_tac_deref_write_case_const_addr_single(0x101, 0x83);
	test_gen_tac_deref_write_case_const_addr_single(0x102, 0x23);
	test_gen_tac_deref_write_case_const_addr_single(0x103, 0x04);
}

void test_gen_tac_deref_write_16bit_case_const_addr() {

	status_test_codegen_tac("Deref - 16 bit write - const Address");

	const uint16_t address = 0x100;
	const uint16_t value = 0x3893;

	char snippet[200];
	sprintf(snippet, "fn main() -> int { *uint16 x = %d; *x = %d; return 0; }", address, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 40);

	const uint16_t actual = vmcu_system_read_data16(system, address);

	assert(actual == 0x3893);

	vmcu_system_dtor(system);
}
