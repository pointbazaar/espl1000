#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

void test_gen_tac_massignstmt_case_const_addr_1_byte() {

	status_test_codegen_tac("MAssignStmt - const Address (1 byte)");

	for (int8_t value = 0; value < 5; value++) {

		char snippet[200];
		sprintf(snippet, "fn main() -> int { [0x100, 1] = %d; return 0; }", value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 20);

		int8_t actual = vmcu_system_read_data(system, 0x100);

		assert(actual == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_massignstmt_case_const_addr_2_byte() {

	status_test_codegen_tac("MAssignStmt - const Address (2 byte)");

	for (uint16_t value = 392; value < 399; value++) {

		char snippet[200];
		sprintf(snippet, "fn main() -> int { [0x100, 2] = %d; return 0; }", value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 20);

		uint16_t actual = vmcu_system_read_data16(system, 0x100);

		assert(actual == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_massignstmt_case_variable_addr_1_byte() {

	status_test_codegen_tac("MAssignStmt - variable Address (1 byte)");

	const uint8_t addr = 0xc7;
	for (uint8_t offset = 0; offset < 5; offset++) {

		const int8_t value = 0x31;

		char snippet[200];
		char* template = "fn main() -> int { int x = %d; [x + 0xc7, 1] = %d; return 0; }";
		sprintf(snippet, template, offset, value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 40);

		int8_t actual = vmcu_system_read_data(system, addr + offset);

		assert(actual == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_massignstmt_case_variable_addr_2_byte() {

	status_test_codegen_tac("MAssignStmt - variable Address (2 byte)");

	const uint8_t addr = 0xc7;
	for (uint8_t offset = 0; offset < 5; offset++) {

		const uint16_t value = 0x312a;

		char snippet[200];
		char* template = "fn main() -> int { int x = %d; [x + 0xc7, 1] = %d; return 0; }";
		sprintf(snippet, template, offset, value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 40);

		uint16_t actual = vmcu_system_read_data16(system, addr + offset);

		assert(actual == value);

		vmcu_system_dtor(system);
	}
}
