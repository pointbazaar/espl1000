#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

void test_gen_tac_ifstmt_no_else_true_8bit() {

	status_test_codegen_tac("IfStmt (no else) true (8 bit)");

	for (int8_t value1 = 0; value1 < 5; value1++) {
		const int8_t value2 = value1 + 0x11;
		const int8_t value_true = 0x33;
		const int8_t value_false = 0x22;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } return %d; }";
		sprintf(snippet, template, value1, value2, value_true, value_false);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_true);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_ifstmt_no_else_true_16bit() {

	status_test_codegen_tac("IfStmt (no else) true (16 bit)");

	for (uint16_t value1 = 0x0100; value1 < 0x0105; value1++) {
		const uint16_t value2 = value1 + 0x11;
		const int8_t value_true = 0x33;
		const int8_t value_false = 0x22;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } return %d; }";
		sprintf(snippet, template, value1, value2, value_true, value_false);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 30);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_true);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_ifstmt_no_else_false_8bit() {

	status_test_codegen_tac("IfStmt (no else) false (8 bit)");

	for (int8_t value1 = 0; value1 < 5; value1++) {
		const int8_t value2 = value1 + 4;
		const int8_t value_true = 0xab;
		const int8_t value_false = 0xba;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } return %d; }";
		sprintf(snippet, template, value2, value1, value_true, value_false);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_false);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_ifstmt_no_else_false_16bit() {

	status_test_codegen_tac("IfStmt (no else) false (16 bit)");

	for (uint16_t value1 = 0x0100; value1 < 0x0105; value1++) {
		const uint16_t value2 = value1 + 4;
		const uint8_t value_true = 0xab;
		const uint8_t value_false = 0xba;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } return %d; }";
		sprintf(snippet, template, value2, value1, value_true, value_false);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 40);

		uint8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_false);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_ifstmt_with_else_true() {

	status_test_codegen_tac("IfStmt (with else) true");

	for (int8_t value1 = 0; value1 < 5; value1++) {
		const int8_t value2 = value1 + 5;
		const int8_t value_true = 0xab;
		const int8_t value_false = 0xba;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } else { return %d; } }";
		sprintf(snippet, template, value1, value2, value_true, value_false);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_true);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_ifstmt_with_else_false() {

	status_test_codegen_tac("IfStmt (with else) false");

	for (int8_t value1 = 0; value1 < 5; value1++) {
		const int8_t value2 = value1 + 3;
		const int8_t value_true = 0xb;
		const int8_t value_false = value_true + 1;

		char snippet[200];
		const char* template = "fn main() -> int { if %d <= %d { return %d; } else { return %d; } }";
		sprintf(snippet, template, value2, value1, value_true, value_false);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value_false);

		vmcu_system_dtor(system);
	}
}
